/* ========================= fr_helpers.c ========================= */
#include "fr_helpers.h"
#include "network.h"      /* for NN_WIDTH / NN_HEIGHT */
#include "app_config.h"   /* for ALIGN_32 / IN_PSRAM (if provided by your BSP) */

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "stm32n6xx_hal.h"

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

/* ---------- forward-static prototypes (avoid implicit decls) ---------- */
static void  fr_l2_normalize(float *v, int n);
static float cosine_similarity(const float *a, const float *b, int n);
static void  fr_add_sample_normed_copy(const float *emb, int n);
static void  fr_finalize_enrollment(int n);

/* ---------- runtime subject label ---------- */
static char g_fr_subject[32] = "ME";

void fr_set_subject(const char *name) {
  if (name && *name) {
    strncpy(g_fr_subject, name, sizeof(g_fr_subject)-1);
    g_fr_subject[sizeof(g_fr_subject)-1] = 0;
  }
}
const char* fr_get_subject(void) { return g_fr_subject; }

static float g_last_sim      = -1.f;
static int   g_last_is_match = 0;

/* ---------- utilities ---------- */
static inline int clampi(int v, int lo, int hi){ return (v<lo)?lo:((v>hi)?hi:v); }

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

/* NEW: reader API used by pp_thread_fct() to consume a stable snapshot */
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

/* (optional) small gallery for 1:N — keep empty unless you paste refs */
static const float g_ref_emb[][FR_EMB_SIZE] = { /* {128/512 floats}, ... */ };
static const char *g_ref_name[] = { /* "Alice", "Bob" */ };
static const int   g_ref_count = (int)(sizeof(g_ref_name)/sizeof(g_ref_name[0]));

/* ---------- API ---------- */
void fr_init(void)
{
  memset(g_det_frame_snap, 0, sizeof(g_det_frame_snap));
  memset(g_me_samples, 0, sizeof(g_me_samples));
  memset(g_me_centroid, 0, sizeof(g_me_centroid));
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
  return g_last_is_match;
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

/* NEW: expose progress */
int fr_get_enroll_progress(int *count_out, int *target_out)
{
  if (count_out)  *count_out  = g_me_count;
  if (target_out) *target_out = FR_SAMPLES_TARGET;
  return g_enroll_done;
}

/* NEW: effective threshold (global + capped personal) */
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

/* Gate for enrollment sample quality (drop outliers) */
#ifndef FR_ENROLL_SIM_MIN
#define FR_ENROLL_SIM_MIN 0.65f
#endif

/* ========== Build FR input from detector snapshot (bilinear crop) ========== */
void fr_prepare_input_for_det(const od_pp_outBuffer_t *d,
                              float *fr_in, int fr_w, int fr_h,
                              int det_in_w, int det_in_h,
                              int det_idx)
{
  /* Make a square box with +10% margin */
  float cx = d->x_center * det_in_w;
  float cy = d->y_center * det_in_h;
  float bw = d->width    * det_in_w;
  float bh = d->height   * det_in_h;
  float side = fmaxf(bw, bh) * 1.10f;
  float x0 = cx - side * 0.5f;
  float y0 = cy - side * 0.5f;

  const float sx_scale = side / (float)fr_w;
  const float sy_scale = side / (float)fr_h;

  /* Throttled ROI trace */
  {
    static uint32_t last_roi_log = 0;
    uint32_t now = HAL_GetTick();
    if ((now - last_roi_log) >= 1000U) {
      float sideN = fmaxf(d->width, d->height);
      float rdx = fabsf(d->x_center - 0.5f), rdy = fabsf(d->y_center - 0.5f);
      float r_center = sqrtf(rdx*rdx + rdy*rdy);
      printf("[FR-IN][ROI] det#%d conf=%.2f  cx=%.3f cy=%.3f  w=%.3f h=%.3f  sideN=%.3f  r_center=%.3f  (px side=%.1f)\n",
             det_idx, d->conf, d->x_center, d->y_center, d->width, d->height, sideN, r_center, side);
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
  for (int y=0; y<fr_h; ++y) {
    float sy = y0 + (y + 0.5f) * sy_scale;
    int sy0 = (int)floorf(sy);
    int sy1 = sy0 + 1;
    float wy = sy - (float)sy0;

    sy0 = clampi(sy0, 0, det_in_h-1);
    sy1 = clampi(sy1, 0, det_in_h-1);

    for (int x=0; x<fr_w; ++x) {
      float sx = x0 + (x + 0.5f) * sx_scale;
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

      int didx = (y*fr_w + x)*3;
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

/* ---------- single, final definition — hardened + filtered ---------- */
void fr_after_inference_and_decide(const float *emb_in, int emb_len, int det_idx)
{
  (void)det_idx;

  /* 1) Determine usable length safely */
  int n = emb_len;
  if (n <= 0 || n > FR_EMB_SIZE) n = FR_EMB_SIZE;

  /* 2) Quick sanity on incoming embedding */
  float mn=0.f, mx=0.f, mean=0.f, l2=0.f;
  int sane = fr_embedding_stats_and_check(emb_in, n, &mn, &mx, &mean, &l2);
  if (!sane) {
    printf("[FR][ERR] embedding corrupt — skip (l2=%g mn=%g mx=%g mean=%g, n=%d)\n",
           (double)l2, (double)mn, (double)mx, (double)mean, n);
    return; /* do not touch votes/state */
  }

  /* 3) Copy and normalize */
  static float emb_f[FR_EMB_SIZE];
  for (int i=0;i<n;i++) emb_f[i] = emb_in[i];
  fr_l2_normalize(emb_f, n);

  /* 4) Enrollment path */
  if (!g_enroll_done) {
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

  /* 5) Runtime verdict */
  float sim_me = cosine_similarity(emb_f, g_me_centroid, n);
  g_last_sim = sim_me;

  float thr_base = g_match_thr;
  float thr_eff  = fr_get_effective_threshold(); /* personal or base (capped) */
  g_last_is_match = (sim_me >= thr_eff);

  printf("[FR][VERDICT] sim=%.3f  thr_eff=%.3f (base=%.3f)  emb[l2_in=%.3f mn=% .3f mx=% .3f mean=% .3f n=%d]  -> %s\n",
         sim_me, thr_eff, thr_base, sqrtf(l2), mn, mx, mean, n,
         g_last_is_match ? "MATCH" : "NO-MATCH");
}

/* ---------- static helpers ---------- */
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
