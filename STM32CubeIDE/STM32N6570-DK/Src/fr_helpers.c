/* ========================= fr_helpers.c ========================= */
#include "fr_helpers.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "stm32n6xx_hal.h"

/* ===== snapshot of detector input (NHWC FP32 [0..1]) ===== */
static float g_last_frame[NN_WIDTH * NN_HEIGHT * 3];

/* ===== enrollment (centroid in RAM) ===== */
#define FR_SAMPLES_TARGET 10  /* number of samples to build the centroid */

static float g_me_samples[FR_SAMPLES_TARGET][FR_EMB_SIZE];
static float g_me_centroid[FR_EMB_SIZE];
static int   g_me_count       = 0;
static int   g_enroll_done    = 0;

/* runtime-configurable match threshold & per-detection enroll gate */
static float g_match_thr   = FR_DEFAULT_MATCH_THR;
static int   g_enroll_gate = 1;  /* set per detection by fr_set_enroll_gate() */

/* (optional) small gallery for 1:N (paste yours if you want) */
static const float g_ref_emb[][FR_EMB_SIZE] = {
  /* {512 floats}, ... */
};
static const char *g_ref_name[] = {
  /* "Alice", "Bob" */
};
static const int g_ref_count = (int)(sizeof(g_ref_name)/sizeof(g_ref_name[0]));

/* ---------- utils ---------- */
static inline int clampi(int v, int lo, int hi){ return (v<lo)?lo:((v>hi)?hi:v); }

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
  float *dst = g_me_samples[g_me_count];
  for (int i=0;i<n;i++) dst[i] = emb[i];
  fr_l2_normalize(dst, n);
  g_me_count++;
  printf("[ENROLL] stored sample %d/%d\r\n", g_me_count, FR_SAMPLES_TARGET);
}

static void fr_finalize_enrollment(int n)
{
  for (int i=0;i<n;i++) g_me_centroid[i] = 0.f;
  for (int k=0;k<g_me_count;k++)
    for (int i=0;i<n;i++)
      g_me_centroid[i] += g_me_samples[k][i];
  float inv = 1.0f / (float)g_me_count;
  for (int i=0;i<n;i++) g_me_centroid[i] *= inv;
  fr_l2_normalize(g_me_centroid, n);
  g_enroll_done = 1;
  printf("[ENROLL] completed with %d samples. Centroid ready.\r\n", g_me_count);
}

/* ---------- runtime config ---------- */
void fr_set_match_threshold(float thr) { g_match_thr = thr; }
void fr_set_enroll_gate(int ok)        { g_enroll_gate = ok; }

void fr_reset_enrollment(void)
{
  memset(g_me_samples, 0, sizeof(g_me_samples));
  memset(g_me_centroid, 0, sizeof(g_me_centroid));
  g_me_count    = 0;
  g_enroll_done = 0;
  printf("[ENROLL] reset. Need %d fresh samples.\r\n", FR_SAMPLES_TARGET);
}

/* ---------- API ---------- */
void fr_init(void)
{
  memset(g_last_frame, 0, sizeof(g_last_frame));
  fr_reset_enrollment();
  g_match_thr   = FR_DEFAULT_MATCH_THR;
  g_enroll_gate = 1;

  printf("[FR][CFG] subject=\"%s\" match_thr=%.2f  enroll: conf>=%.2f  minSide>=%.2f  centerTol<=%.2f\r\n",
         FR_SUBJECT_NAME, g_match_thr, FR_ENROLL_CONF_MIN, FR_ENROLL_MIN_SIDE, FR_ENROLL_CENTER_TOL);
}

void fr_update_frame_snapshot(const float *src_nhwc, uint32_t bytes)
{
  const uint32_t cap = sizeof(g_last_frame);
  if (bytes > cap) bytes = cap;
  memcpy(g_last_frame, src_nhwc, bytes);
}

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

  uint32_t t0 = HAL_GetTick();

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
        float v = g_last_frame[idx00+c]*w00 +
                  g_last_frame[idx01+c]*w01 +
                  g_last_frame[idx10+c]*w10 +
                  g_last_frame[idx11+c]*w11;
        /* Convert [0..1] -> [-1..1] for most FR backbones */
        fr_in[didx+c] = v*2.f - 1.f;
      }
    }
  }

  uint32_t t1 = HAL_GetTick();

  /* quick stats & timing */
  {
    const int N = fr_w * fr_h * 3;
    float mn =  1e30f, mx = -1e30f, sum = 0.f; int nan_cnt = 0;
    for (int k = 0; k < N; ++k) {
      float v = fr_in[k];
      if (v != v) { nan_cnt++; continue; }
      if (v < mn) mn = v;
      if (v > mx) mx = v;
      sum += v;
    }
    float mean = (N - nan_cnt) ? (sum / (float)(N - nan_cnt)) : 0.f;
    printf("[FR-IN] det#%d stats: min=% .3f  max=% .3f  mean=% .3f  NaN=%d  size=%dx%dx3\r\n",
           det_idx, mn, mx, mean, nan_cnt, fr_w, fr_h);
    printf("[TIM] CROP %3d took %lums\r\n", det_idx, (unsigned long)(t1 - t0));
  }
}

void fr_after_inference_and_decide(const int8_t *emb_q, int emb_len, int det_idx)
{
  /* int8 -> float in [-1..1] (adjust if your scale differs) */
  static float emb_f[FR_EMB_SIZE];
  int n = (emb_len < FR_EMB_SIZE) ? emb_len : FR_EMB_SIZE;

  float mn =  1e30f, mx = -1e30f, sum = 0.f, l2 = 0.f;
  for (int i=0;i<n;i++) {
    emb_f[i] = (float)emb_q[i] / 128.0f;
    float v = emb_f[i];
    if (v < mn) mn = v;
    if (v > mx) mx = v;
    sum += v;
    l2  += v*v;
  }
  float mean = (n > 0) ? (sum / (float)n) : 0.f;
  printf("[FR-OUT] det#%d emb%d (int8->f): min=% .5f max=% .5f mean=% .5f L2=%.5f\r\n",
         det_idx, n, mn, mx, mean, sqrtf(l2));

  /* Enrollment then match */
  if (!g_enroll_done) {
    if (!g_enroll_gate) {
      printf("[ENROLL] skipped this detection (gate=0)\r\n");
      return;
    }
    fr_add_sample_normed_copy(emb_f, n);
    if (g_me_count >= FR_SAMPLES_TARGET) {
      fr_finalize_enrollment(n);
    } else {
      printf("[ENROLL] waiting for %d more sample(s)...\r\n",
             FR_SAMPLES_TARGET - g_me_count);
    }
    return;
  }

  /* Compare to centroid (subject) */
  float sim_me = cosine_similarity(emb_f, g_me_centroid, n);
  printf("[FR] det#%d %s? sim=%.3f (thr=%.2f) -> %s\r\n",
         det_idx, FR_SUBJECT_NAME, sim_me, g_match_thr,
         (sim_me >= g_match_thr) ? "YES" : "NO");
  if (sim_me >= g_match_thr) {
    printf(">>> %s FACE FOUND (sim=%.3f >= thr=%.2f)\r\n", FR_SUBJECT_NAME, sim_me, g_match_thr);
  } else {
    printf(">>> WRONG FACE FOUND (sim=%.3f < thr=%.2f)\r\n", sim_me, g_match_thr);
  }

  /* Optional 1:N gallery */
  if (g_ref_count > 0) {
    int   best_idx = -1;
    float best_sim = -2.0f;
    for (int r = 0; r < g_ref_count; ++r) {
      float s = cosine_similarity(emb_f, g_ref_emb[r], n);
      if (s > best_sim) { best_sim = s; best_idx = r; }
    }
    if (best_idx >= 0) {
      printf("[FR] det#%d gallery best: %s (sim=%.3f)\r\n",
             det_idx, g_ref_name[best_idx], best_sim);
    }
  }
}

void fr_check_alias(const void *in_ptr, const void *out_ptr)
{
  if (in_ptr == out_ptr) {
    printf("[FR][WARN] INPUT and OUTPUT share the same address (%p) — this can corrupt data!\r\n",
           in_ptr);
  }
}
