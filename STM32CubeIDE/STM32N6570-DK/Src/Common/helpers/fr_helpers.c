/* ========================= fr_helpers.c ========================= */
/* Stable FR pipeline: snapshot -> square+margin crop -> FR input
 * -> embedding (float or int8) -> dequantize+L2 (if needed)
 * -> cosine -> EMA + hysteresis -> stable Muazzam/Unknown
 */

#include "fr_helpers.h"
#include "network.h"      /* for NN_WIDTH / NN_HEIGHT */
#include "app_config.h"   /* FR_* macros, USE_DCACHE, etc. */

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "stm32n6xx_hal.h"

/* ---------- Tuning (override via -D or app_config.h if you want) ---------- */
#ifndef FR_THR_ON
#define FR_THR_ON   0.77f   /* enter-known threshold (cosine) */
#endif
#ifndef FR_THR_OFF
#define FR_THR_OFF  0.73f   /* exit-known threshold (must be < FR_THR_ON) */
#endif
#ifndef FR_EMA_A
#define FR_EMA_A    0.25f   /* EMA smoothing factor [0..1], higher = snappier */
#endif
#ifndef FR_KDROP
#define FR_KDROP    3       /* consecutive frames below OFF to drop identity */
#endif
#ifndef FR_CROP_MARGIN
#define FR_CROP_MARGIN 1.10f /* square crop margin multiplier */
#endif
#ifndef FR_ENROLL_SIM_MIN
#define FR_ENROLL_SIM_MIN 0.65f /* drop noisy enroll sample if below this to mean */
#endif
/* ---------- Tuning (override via -D or app_config.h if you want) ---------- */
#ifndef FR_THR_ON
#define FR_THR_ON   0.77f
#endif

/* Global frame snapshot buffer (used by fr_get_frame_snapshot / fr_clear_frame_snapshot) */
struct {
    uint8_t *ptr;
    uint32_t len;
} g_frame_snapshot = { NULL, 0 };

/* ---------- local DCACHE helpers (TU-private) ---------- */
static inline void fr_dcache_align_range(void **addr, size_t *len)
{
#if defined(USE_DCACHE)
  uintptr_t a = (uintptr_t)(*addr);
  uintptr_t a0 = a & ~((uintptr_t)31);                 /* align down to 32B */
  size_t extra = (size_t)(a - a0);
  size_t l0 = *len + extra;
  l0 = (l0 + 31U) & ~31U;                              /* align up to 32B */
  *addr = (void *)a0;
  *len  = l0;
#else
  (void)addr; (void)len;
#endif
}
static inline void FR_DCACHE_Clean(void *addr, size_t len)
{
#if defined(USE_DCACHE)
  fr_dcache_align_range(&addr, &len);
  SCB_CleanDCache_by_Addr(addr, (int)len);
#else
  (void)addr; (void)len;
#endif
}

/* ---------- small utilities ---------- */
static inline int clampi(int v, int lo, int hi){ return (v<lo)?lo:((v>hi)?hi:v); }

/* ---------- forward-static prototypes (avoid implicit decls) ---------- */
static void  fr_l2_normalize(float *v, int n);
static float cosine_similarity(const float *a, const float *b, int n);
static void  fr_add_sample_normed_copy(const float *emb, int n);
static void  fr_finalize_enrollment(int n);

/* helpers requested */
static void  fr_make_square_crop(int *x, int *y, int *w, int *h, int img_w, int img_h);
void         fr_update_identity_state(float sim, int best_ref, int det_idx);
static void  fr_average_refs(const void *set, int n, float *out); /* EmbRec-like */

/* optional: INT8 -> float dequant path (for FR_OUT_IS_INT8) */
#if FR_OUT_IS_INT8
typedef struct {
  float scale;   /* if you know the real scale, set it at runtime */
  int   zp;      /* zero-point; FaceNet INT8 exports are usually symmetric (0) */
} fr_quant_t;
static fr_quant_t g_out_q = { .scale = 1.0f, .zp = 0 }; /* set real values if available */

static void fr_from_int8_to_unit(const int8_t *src, float *dst, int n)
{
  for (int i = 0; i < n; ++i) dst[i] = g_out_q.scale * ((int)src[i] - g_out_q.zp);
  fr_l2_normalize(dst, n);
}
#endif /* FR_OUT_IS_INT8 */

/* ---------- runtime subject label ---------- */
static char g_fr_subject[32] = "ME";
void fr_set_subject(const char *name) {
  if (name && *name) {
    strncpy(g_fr_subject, name, sizeof(g_fr_subject)-1);
    g_fr_subject[sizeof(g_fr_subject)-1] = 0;
  }
}
const char* fr_get_subject(void) { return g_fr_subject; }

/* ---------- result latch (exposed via fr_get_last_match) ---------- */
static float g_last_sim      = -1.f;
static int   g_last_is_match = 0;

/* ===== snapshot of detector input (NHWC FP32 [0..1]) =====
 * NOTE: keep this in PSRAM (if available) and 32B aligned.
 */
#ifndef ALIGN_32
#define ALIGN_32 __attribute__((aligned(32)))
#endif
#ifndef IN_PSRAM
#define IN_PSRAM
#endif

/* Ping-pong buffers to avoid read-while-write races */
static float g_det_frame_snap[2][NN_WIDTH * NN_HEIGHT * 3] ALIGN_32 IN_PSRAM;
static volatile uint32_t g_snap_idx = 0;

void fr_clear_frame_snapshot(void)
{
    printf("[FR][Snapshot] Cleared last frame snapshot.\r\n");
    if (g_frame_snapshot.ptr) {
        memset((void*)g_frame_snapshot.ptr, 0, g_frame_snapshot.len);
    }
}

/* External caller (nn thread) pushes the *float* detector input here. */
void fr_update_frame_snapshot(const float *src_nhwc, uint32_t bytes)
{
  uint32_t write_idx = 1u - g_snap_idx;               /* write into the non-active bank */
  memcpy(g_det_frame_snap[write_idx], src_nhwc, (size_t)bytes);
  FR_DCACHE_Clean(g_det_frame_snap[write_idx], (size_t)bytes);  /* CPU write visibility */
  __DMB();                                            /* ensure data visible before publishing index */
  g_snap_idx = write_idx;                             /* publish */
  __DMB();
}

/* Reader API used by pp_thread_fct() to consume a stable snapshot */
void fr_get_frame_snapshot(const void **ptr, uint32_t *len)
{
  if (ptr) *ptr = NULL;
  if (len) *len = 0;

  __DMB();
  uint32_t read_idx = g_snap_idx;                     /* single atomic read */
  __DMB();

  if (ptr) *ptr = (const void *)g_det_frame_snap[read_idx];
  if (len) *len = (uint32_t)(NN_WIDTH * NN_HEIGHT * 3 * sizeof(float));
}

/* ===== enrollment (centroid in RAM) ===== */
#define FR_SAMPLES_TARGET 10  /* number of samples to build the centroid */

static float g_me_samples[FR_SAMPLES_TARGET][FR_EMB_SIZE];
static float g_me_centroid[FR_EMB_SIZE];
static int   g_me_count    = 0;
static int   g_enroll_done = 0;

/* runtime-configurable match threshold & per-detection enroll gate */
static float g_match_thr   = FR_DEFAULT_MATCH_THR;
static int   g_enroll_gate = 1;  /* set per detection by fr_set_enroll_gate() */

/* Personal, data-driven threshold computed after enrollment (mu-1.5*sigma), capped later */
static float g_personal_thr = 0.0f;  /* 0 means “not computed yet” */

/* (optional) small gallery for 1:N — keep empty unless you paste refs
 * If you link embeddings_table.c, you can extern the set there when needed. */
typedef struct { const char* name; const float* data; int dim; } EmbRec;
/* extern const EmbRec g_ref_set[]; */
/* extern const int g_ref_set_count; */

/* ---------- EMA + Hysteresis state (per det_idx) ---------- */
typedef struct {
  float ema_sim;
  int   is_known;
  int   below_cnt;
  int   best_ref;
} fr_track_t;

#define FR_TRACK_MAX 8
static fr_track_t g_track[FR_TRACK_MAX];

/* ---------- API ---------- */
void fr_init(void)
{
  memset(g_det_frame_snap, 0, sizeof(g_det_frame_snap));
  memset(g_me_samples, 0, sizeof(g_me_samples));
  memset(g_me_centroid, 0, sizeof(g_me_centroid));
  memset(g_track, 0, sizeof(g_track));
  g_me_count     = 0;
  g_enroll_done  = 0;
  g_match_thr    = FR_DEFAULT_MATCH_THR;
  g_enroll_gate  = 1;
  g_personal_thr = 0.0f;

  /* Use compile-time label if provided, else runtime subject string */
#ifdef FR_SUBJECT_NAME
  const char *subj = FR_SUBJECT_NAME;
#else
  const char *subj = g_fr_subject;
#endif

  printf("[FR][CFG] subject=\"%s\" match_thr=%.2f  enroll: conf>=%.2f  minSide>=%.2f  centerTol<=%.2f\r\n",
         subj, g_match_thr, FR_ENROLL_CONF_MIN, FR_ENROLL_MIN_SIDE, FR_ENROLL_CENTER_TOL);
}

/* Public gate setters/getters */
int  fr_is_enrolled(void)               { return g_enroll_done; }

int fr_get_last_match(float *sim_out)
{
  if (sim_out) *sim_out = g_last_sim;
  return g_last_is_match; /* NOTE: this reflects the *stable* hysteresis decision */
}

void fr_set_match_threshold(float thr)  { g_match_thr = thr; }
void fr_set_enroll_gate(int ok)         { g_enroll_gate = ok; }

void fr_reset_enrollment(void)
{
  memset(g_me_samples, 0, sizeof(g_me_samples));
  memset(g_me_centroid, 0, sizeof(g_me_centroid));
  g_me_count     = 0;
  g_enroll_done  = 0;
  g_personal_thr = 0.0f;
  printf("[ENROLL] reset. Need %d fresh samples.\r\n", FR_SAMPLES_TARGET);
}

/* Safety: warn once if FR input & output alias the same address */
void fr_check_alias(const void *in_ptr, const void *out_ptr)
{
  static int warned = 0;
  if (in_ptr == out_ptr && !warned) {
    warned = 1;
    printf("[FR][WARN] IN and OUT alias — using shadow copy is safe, but prefer re-export with separate I/O.\r\n");
  }
}

/* Expose progress */
int fr_get_enroll_progress(int *count_out, int *target_out)
{
  if (count_out)  *count_out  = g_me_count;
  if (target_out) *target_out = FR_SAMPLES_TARGET;
  return g_enroll_done;
}

/* Effective threshold (global + capped personal) */
float fr_get_effective_threshold(void)
{
  float base = g_match_thr;                         /* global knob (e.g., 0.80f) */
  float pers = (g_personal_thr > 0.f) ? g_personal_thr : base;
  /* Soft cap: personal thr cannot exceed base + 0.03; and never below base */
  float capped = fminf(pers, base + 0.03f);
  if (capped < base) capped = base;
  return capped;
}

/* ---------- helpers for sanity checks ---------- */
static int fr_embedding_stats_and_check(const float *v, int n,
                                        float *mn, float *mx, float *mean, float *l2)
{
  float _mn =  1e30f, _mx = -1e30f, sum = 0.f, s2 = 0.f;
  int nan_cnt = 0, big_cnt = 0;

  for (int i=0;i<n;i++) {
    float x = v[i];
    if (!(x == x)) { nan_cnt++; continue; }          /* NaN */
    if (x < _mn) _mn = x;
    if (x > _mx) _mx = x;
    sum += x;
    s2  += x*x;
    if (fabsf(x) > 16.f) big_cnt++;                  /* absurd for FR embeddings */
  }

  if (mn)   *mn   = (nan_cnt==n) ? 0.f : _mn;
  if (mx)   *mx   = (nan_cnt==n) ? 0.f : _mx;
  if (mean) *mean = (nan_cnt==n) ? 0.f : sum / (float)(n - nan_cnt);
  if (l2)   *l2   = s2;

  /* corrupt if any NaN, or many absurdly big values, or min/max super huge */
  if (nan_cnt > 0) return 0;
  if (big_cnt > 0) return 0;
  if (fabsf(_mn) > 1e4f || fabsf(_mx) > 1e4f) return 0;
  return 1;
}

/* ==================== Crop + Resize (build FR input) ==================== */

static void fr_make_square_crop(int *x, int *y, int *w, int *h, int img_w, int img_h)
{
  /* Expand to square with margin, clamp to image bounds */
  int side = (*w > *h) ? *w : *h;
  side = (int)(side * FR_CROP_MARGIN);
  int cx = *x + *w/2;
  int cy = *y + *h/2;

  int nx = cx - side/2;
  int ny = cy - side/2;
  if (nx < 0) nx = 0;
  if (ny < 0) ny = 0;
  if (nx + side > img_w) side = img_w - nx;
  if (ny + side > img_h) side = img_h - ny;

  *x = nx; *y = ny; *w = side; *h = side;
}

void fr_prepare_input_for_det(const od_pp_outBuffer_t *d,
                              float *fr_in, int fr_w, int fr_h,
                              int det_in_w, int det_in_h,
                              int det_idx)
{
  /* Convert normalized BlazeFace box to pixels */
  float fx0 = (d->x_center - d->width * 0.5f) * det_in_w;
  float fy0 = (d->y_center - d->height* 0.5f) * det_in_h;
  float fww = d->width  * det_in_w;
  float fhh = d->height * det_in_h;
  int x = (int)floorf(fx0);
  int y = (int)floorf(fy0);
  int w = (int)ceilf(fww);
  int h = (int)ceilf(fhh);

  fr_make_square_crop(&x, &y, &w, &h, det_in_w, det_in_h);

  const float sx_scale = (float)w / (float)fr_w;
  const float sy_scale = (float)h / (float)fr_h;

  /* Throttled ROI trace */
  {
    static uint32_t last_roi_log = 0;
    uint32_t now = HAL_GetTick();
    if ((now - last_roi_log) >= 1000U) {
      float sideN = fmaxf(d->width, d->height);
      float rdx = fabsf(d->x_center - 0.5f), rdy = fabsf(d->y_center - 0.5f);
      float r_center = sqrtf(rdx*rdx + rdy*rdy);
      printf("[FR-IN][ROI] det#%d conf=%.2f  cx=%.3f cy=%.3f  w=%.3f h=%.3f  sideN=%.3f  r_center=%.3f  (px side=%d)\n",
             det_idx, d->conf, d->x_center, d->y_center, d->width, d->height, sideN, r_center, w);
      last_roi_log = now;
    }
  }

  uint32_t t0 = HAL_GetTick();

  /* Lock-in which snapshot we use for this whole crop */
  __DMB();
  uint32_t snap_idx = g_snap_idx;      /* single atomic read is enough */
  __DMB();
  const float *snap = g_det_frame_snap[snap_idx];

  /* Bilinear sample from the *snapshot* buffer (NHWC FP32 in [0..1]) */
  for (int yy=0; yy<fr_h; ++yy) {
    float sy = (float)y + (yy + 0.5f) * sy_scale;
    int sy0 = (int)floorf(sy);
    int sy1 = sy0 + 1;
    float wy = sy - (float)sy0;

    sy0 = clampi(sy0, 0, det_in_h-1);
    sy1 = clampi(sy1, 0, det_in_h-1);

    for (int xx=0; xx<fr_w; ++xx) {
      float sx = (float)x + (xx + 0.5f) * sx_scale;
      int sx0 = (int)floorf(sx);
      int sx1 = sx0 + 1;
      float wx = sx - (float)sx0;

      sx0 = clampi(sx0, 0, det_in_w-1);
      sx1 = clampi(sx1, 0, det_in_w-1);

      int idx00 = (sy0*det_in_w + sx0)*3;
      int idx01 = (sy0*det_in_w + sx1)*3;
      int idx10 = (sy1*det_in_w + sx0)*3;
      int idx11 = (sy1*det_in_w + sx1)*3;

      float w00 = (1.f-wx)*(1.f-wy);
      float w01 = (     wx)*(1.f-wy);
      float w10 = (1.f-wx)*(     wy);
      float w11 = (     wx)*(     wy);

      int didx = (yy*fr_w + xx)*3;
      for (int c=0;c<3;c++){
        float v = snap[idx00+c]*w00 +
                  snap[idx01+c]*w01 +
                  snap[idx10+c]*w10 +
                  snap[idx11+c]*w11;
        /* Convert [0..1] -> [-1..1] for most FR backbones */
        fr_in[didx+c] = v*2.f - 1.f;
      }
    }
  }

  uint32_t t1 = HAL_GetTick();

  /* quick stats & timing (kept; useful to spot NaNs) */
  {
    const int N = fr_w * fr_h * 3;
    float mn =  1e30f, mx = -1e30f, sum = 0.f; int nan_cnt = 0;
    for (int k = 0; k < N; ++k) {
      float v = fr_in[k];
      if (!(v == v)) { nan_cnt++; continue; }
      if (v < mn) mn = v;
      if (v > mx) mx = v;
      sum += v;
    }
    float mean = (N - nan_cnt) ? (sum / (float)(N - nan_cnt)) : 0.f;
    printf("[FR-IN] det#%d stats: min=% .3f  max=% .3f  mean=% .3f  NaN=%d  size=%dx%dx3  t=%lums\n",
           det_idx, mn, mx, mean, nan_cnt, fr_w, fr_h, (unsigned long)(t1 - t0));
  }
}

/* ==================== Inference result → decision ==================== */

void fr_after_inference_and_decide(const float *emb_in_maybe_float, int emb_len, int det_idx)
{
  /* 1) Determine usable length safely */
  int n = emb_len;
  if (n <= 0 || n > FR_EMB_SIZE) n = FR_EMB_SIZE;

  /* 2) Dequantize (if INT8) / Copy and sanity-check */
  static float emb_f[FR_EMB_SIZE];
#if FR_OUT_IS_INT8
  /* emb_in points to int8 memory in this build */
  fr_from_int8_to_unit((const int8_t*)emb_in_maybe_float, emb_f, n);
#else
  for (int i=0;i<n;i++) emb_f[i] = emb_in_maybe_float[i];
  fr_l2_normalize(emb_f, n);
#endif

  float mn=0.f, mx=0.f, mean=0.f, l2=0.f;
  int sane = fr_embedding_stats_and_check(emb_f, n, &mn, &mx, &mean, &l2);
  if (!sane) {
    printf("[FR][ERR] embedding corrupt — skip (l2=%g mn=%g mx=%g mean=%g, n=%d)\n",
           (double)l2, (double)mn, (double)mx, (double)mean, n);
    return; /* do not touch votes/state */
  }

  /* 3) Enrollment path (centroid) */
  if (!g_enroll_done) {
    if (!g_enroll_gate) return; /* external gate says not now */

    /* If we already have a couple samples, compare to running mean and drop outliers */
    if (g_me_count >= 2) {
      float tmp_centroid[FR_EMB_SIZE];
      for (int i=0;i<n;i++) tmp_centroid[i] = 0.f;
      for (int k=0;k<g_me_count;k++)
        for (int i=0;i<n;i++)
          tmp_centroid[i] += g_me_samples[k][i];
      float inv = 1.0f / (float)g_me_count;
      for (int i=0;i<n;i++) tmp_centroid[i] *= inv;
      fr_l2_normalize(tmp_centroid, n);
      float s2m = cosine_similarity(emb_f, tmp_centroid, n);

      if (s2m < FR_ENROLL_SIM_MIN) {
        printf("[FR][ENROLL] drop noisy sample (sim-to-mean=%.3f < %.2f)\n", s2m, FR_ENROLL_SIM_MIN);
        return;
      } else {
        printf("[FR][ENROLL] accept sample (sim-to-mean=%.3f)\n", s2m);
      }
    }

    fr_add_sample_normed_copy(emb_f, n);

    if (g_me_count >= FR_SAMPLES_TARGET) {
      fr_finalize_enrollment(n);
#ifdef FR_SUBJECT_NAME
      printf("[ENROLL] %s enrolled.\n", FR_SUBJECT_NAME);
#else
      printf("[ENROLL] %s enrolled.\n", g_fr_subject);
#endif
    }
    return;
  }

  /* 4) Runtime verdict (cosine to centroid) */
  float sim_me = cosine_similarity(emb_f, g_me_centroid, n);
  g_last_sim = sim_me;

  /* Feed into temporal stabilizer */
  fr_update_identity_state(sim_me, /*best_ref=*/0, det_idx);

  /* Report stable decision */
  printf("[FR][VERDICT] det#%d sim=%.3f  thr_eff=%.3f (base=%.3f)  -> %s\n",
         det_idx, sim_me, fr_get_effective_threshold(), g_match_thr,
         g_last_is_match ? "MATCH" : "NO-MATCH");
}

/* ==================== Static helpers ==================== */

static void fr_l2_normalize(float *v, int n)
{
  float s = 0.f; for (int i=0;i<n;i++) s += v[i]*v[i];
  float inv = 1.0f / (sqrtf(s) + 1e-9f);
  for (int i=0;i<n;i++) v[i] *= inv;
}

static float cosine_similarity(const float *a, const float *b, int n)
{
  float dot=0.f, na=0.f, nb=0.f;
  for (int i=0;i<n;i++){ float va=a[i], vb=b[i]; dot+=va*vb; na+=va*va; nb+=vb*vb; }
  float den = sqrtf(na)*sqrtf(nb) + 1e-6f;
  return dot / den;
}

static void fr_add_sample_normed_copy(const float *emb, int n)
{
  if (g_me_count >= FR_SAMPLES_TARGET) return;
  float *dst = g_me_samples[g_me_count];
  for (int i=0;i<n;i++) dst[i] = emb[i];
  fr_l2_normalize(dst, n);
  g_me_count++;
  printf("[ENROLL] stored sample %d/%d\r\n", g_me_count, FR_SAMPLES_TARGET);
}

static void fr_finalize_enrollment(int n)
{
  /* centroid */
  for (int i=0;i<n;i++) g_me_centroid[i] = 0.f;
  for (int k=0;k<g_me_count;k++)
    for (int i=0;i<n;i++)
      g_me_centroid[i] += g_me_samples[k][i];
  float inv = 1.0f / (float)g_me_count;
  for (int i=0;i<n;i++) g_me_centroid[i] *= inv;
  fr_l2_normalize(g_me_centroid, n);

  /* compute a personal threshold from sample similarities */
  float mu = 0.f, m2 = 0.f; int cnt = 0;
  for (int k=0;k<g_me_count;k++){
    float s = cosine_similarity(g_me_samples[k], g_me_centroid, n);
    cnt++;
    float delta = s - mu;
    mu += delta / (float)cnt;
    m2 += delta * (s - mu);
  }
  float var = (cnt > 1) ? (m2 / (float)(cnt - 1)) : 0.f;
  float sd  = sqrtf(fmaxf(0.f, var));

  /* personal thr = mu - 1.5*sd, clamped to [0.65 .. 0.90] */
  float thr_p = mu - 1.5f * sd;
  if (thr_p < 0.65f) thr_p = 0.65f;
  if (thr_p > 0.90f) thr_p = 0.90f;
  g_personal_thr = thr_p;

  g_enroll_done = 1;
  printf("[ENROLL] completed with %d samples. mu=%.3f sd=%.3f personal_thr=%.3f (global=%.3f)\r\n",
         g_me_count, mu, sd, g_personal_thr, g_match_thr);
}

/* ==================== EMA + Hysteresis (stable decision) ==================== */

void fr_update_identity_state(float sim, int best_ref, int det_idx)
{
  fr_track_t *t = &g_track[det_idx & (FR_TRACK_MAX-1)];

  /* init EMA to first observation */
  if (t->ema_sim == 0.f && t->below_cnt == 0 && t->is_known == 0) {
    t->ema_sim = sim;
  } else {
    t->ema_sim = FR_EMA_A * sim + (1.f - FR_EMA_A) * t->ema_sim;
  }

  /* derive thresholds around effective threshold, but never stricter than FR_THR_ON */
  float thr_eff = fr_get_effective_threshold();
  float thr_on  = fmaxf(FR_THR_ON,  thr_eff);
  float thr_off = fminf(FR_THR_OFF, thr_on - 0.02f); /* ensure some gap */

  if (!t->is_known) {
    if (t->ema_sim >= thr_on) {
      t->is_known  = 1;
      t->best_ref  = best_ref;
      t->below_cnt = 0;
    }
  } else {
    if (t->ema_sim < thr_off) {
      if (++t->below_cnt >= FR_KDROP) {
        t->is_known  = 0;
        t->below_cnt = 0;
      }
    } else {
      t->below_cnt = 0;
    }
  }

  /* Latch for external readers */
  g_last_is_match = t->is_known;
}

/* ==================== Optional: reference averaging helper ==================== */
/* Use if you maintain multiple reference embeddings and want a prototype.
 * `set` must be an array of EmbRec { name, data, dim } with length n. */
static void fr_average_refs(const void *set, int n, float *out)
{
  const EmbRec *rs = (const EmbRec *)set;
  if (!rs || n <= 0 || !out) return;

  /* zero */
  for (int i=0;i<FR_EMB_SIZE;i++) out[i] = 0.f;

  int dim = rs[0].dim;
  for (int k=0;k<n;k++) {
    const float *p = rs[k].data;
    for (int i=0;i<dim;i++) out[i] += p[i];
  }
  float inv = 1.0f / (float)n;
  for (int i=0;i<dim;i++) out[i] *= inv;
  fr_l2_normalize(out, dim);
}
