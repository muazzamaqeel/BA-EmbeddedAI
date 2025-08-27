/* app_postprocess.c — BlazeFace (128x128) custom postprocess
 * - Decodes 896 anchors (512 + 384) and 16-reg values per anchor
 * - Expects scores already passed through sigmoid() in pp_thread_fct
 */

#include "app_postprocess.h"
#include "app_config.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#if POSTPROCESS_TYPE == POSTPROCESS_CUSTOM

#ifndef NN_WIDTH
#  define NN_WIDTH  128
#  define NN_HEIGHT 128
#endif

/* =================== Tunables =================== */
#define BLZ_SCORE_TH     (0.50f)
#define BLZ_NMS_IOU      (0.30f)
#ifndef AI_OD_PP_MAX_BOXES_LIMIT
#  define AI_OD_PP_MAX_BOXES_LIMIT 10
#endif
#define BLZ_MAX_OUT      (AI_OD_PP_MAX_BOXES_LIMIT)

/* BlazeFace head sizes */
#define BLZ_L_ANCHORS    512   /* 16x16x2 */
#define BLZ_S_ANCHORS    384   /*  8x8x6 */
#define BLZ_TOTAL        (BLZ_L_ANCHORS + BLZ_S_ANCHORS)

/* How regressors were trained: typical SSD-style scales */
#define OFFSET_SCALE_X   (1.0f / 128.0f)
#define OFFSET_SCALE_Y   (1.0f / 128.0f)
#define SIZE_SCALE_W     (1.0f / 128.0f)
#define SIZE_SCALE_H     (1.0f / 128.0f)

/* =================== Types/Storage =================== */

typedef struct {
  float cx, cy, w, h;  /* normalized [0..1] */
  float score;
  int   class_id;
} my_box_t;

static my_box_t       s_cand[BLZ_TOTAL];
static unsigned char  s_suppressed[BLZ_TOTAL];
static od_pp_outBuffer_t s_out[BLZ_MAX_OUT];

/* Anchor arrays (normalized) */
static float s_anc_cx[BLZ_TOTAL], s_anc_cy[BLZ_TOTAL];
static float s_anc_w [BLZ_TOTAL], s_anc_h [BLZ_TOTAL];
static int   s_anchors_built = 0;

/* =================== Helpers =================== */
static inline float clampf(float v, float lo, float hi){ return v < lo ? lo : (v > hi ? hi : v); }

static inline float iou_xywh(const my_box_t *a, const my_box_t *b)
{
  float ax1=a->cx-0.5f*a->w, ay1=a->cy-0.5f*a->h;
  float ax2=a->cx+0.5f*a->w, ay2=a->cy+0.5f*a->h;
  float bx1=b->cx-0.5f*b->w, by1=b->cy-0.5f*b->h;
  float bx2=b->cx+0.5f*b->w, by2=b->cy+0.5f*b->h;

  float ix1=fmaxf(ax1,bx1), iy1=fmaxf(ay1,by1);
  float ix2=fminf(ax2,bx2), iy2=fminf(ay2,by2);
  float iw=fmaxf(0.f, ix2-ix1), ih=fmaxf(0.f, iy2-iy1);
  float inter=iw*ih, uni=(ax2-ax1)*(ay2-ay1)+(bx2-bx1)*(by2-by1)-inter+1e-6f;
  return inter/uni;
}

/* =================== Anchor generation ===================
 * For 128x128 BlazeFace Front:
 *  - L layer: 16x16, 2 anchors per cell (rough small/med faces)
 *  - S layer:  8x8,  6 anchors per cell (larger faces)
 * Aspect ratio ~1, scales chosen to cover practical face sizes.
 * (These are sane defaults; tune if needed.)
 */
static void build_blazeface_anchors(void)
{
  if (s_anchors_built) return;

  /* Layer L (16x16, 2 per cell) */
  {
    const int fm = 16, na = 2;
    const float scales[2] = { 0.15f, 0.30f }; /* approx small/medium */
    int idx = 0;
    for (int y=0; y<fm; ++y) {
      for (int x=0; x<fm; ++x) {
        float cx = (x + 0.5f) / (float)fm;
        float cy = (y + 0.5f) / (float)fm;
        for (int a=0; a<na; ++a) {
          s_anc_cx[idx] = cx;
          s_anc_cy[idx] = cy;
          s_anc_w [idx] = scales[a];
          s_anc_h [idx] = scales[a];
          idx++;
        }
      }
    }
  }

  /* Layer S (8x8, 6 per cell) */
  {
    const int fm = 8, na = 6;
    const float scales[6] = { 0.20f, 0.32f, 0.45f, 0.60f, 0.75f, 0.90f };
    int base = BLZ_L_ANCHORS;
    int idx = base;
    for (int y=0; y<fm; ++y) {
      for (int x=0; x<fm; ++x) {
        float cx = (x + 0.5f) / (float)fm;
        float cy = (y + 0.5f) / (float)fm;
        for (int a=0; a<na; ++a) {
          s_anc_cx[idx] = cx;
          s_anc_cy[idx] = cy;
          s_anc_w [idx] = scales[a];
          s_anc_h [idx] = scales[a];
          idx++;
        }
      }
    }
  }

  s_anchors_built = 1;
  printf("BlazeFace anchors built: %d (512+384)\n", BLZ_TOTAL);
}

/* Decode a single anchor’s 16-regression vector into a normalized box.
 * We use SSD-ish decode: offsets are relative to anchor center/size,
 * and sizes are exponentiated residuals.
 */
static inline void decode_one(const float *r, int anc_idx, my_box_t *b)
{
  /* regressors */
  float dx = r[0] * OFFSET_SCALE_X;
  float dy = r[1] * OFFSET_SCALE_Y;
  float dw = r[2] * SIZE_SCALE_W;
  float dh = r[3] * SIZE_SCALE_H;

  /* anchor */
  float acx = s_anc_cx[anc_idx], acy = s_anc_cy[anc_idx];
  float aw  = s_anc_w [anc_idx], ah  = s_anc_h [anc_idx];

  /* SSD-style decode */
  float cx = acx + dx * aw;
  float cy = acy + dy * ah;
  float w  = aw  * expf(dw);
  float h  = ah  * expf(dh);

  /* clamp to [0..1] and reject nonsense */
  b->cx = clampf(cx, 0.f, 1.f);
  b->cy = clampf(cy, 0.f, 1.f);
  b->w  = clampf(w,  0.f, 1.f);
  b->h  = clampf(h,  0.f, 1.f);
}

/* Collect candidates ≥ score threshold from a head into dst; returns count */
static int collect_head_into(const float *reg, const float *score,
                             int n_anchors, int anc_offset, my_box_t *dst)
{
  int n = 0;
  for (int i=0; i<n_anchors; ++i) {
    float s = score[i];
    if (s < BLZ_SCORE_TH) continue;

    const float *r = &reg[i*16];   /* 16 values per anchor */
    my_box_t b;
    decode_one(r, anc_offset + i, &b);

    /* sanity: reject tiny */
    if (b.w < 0.02f || b.h < 0.02f) continue;

    b.score = s;
    b.class_id = 0;
    dst[n++] = b;
  }
  return n;
}

/* ====================================================== */

int32_t app_postprocess_init(void *params_postprocess)
{
  (void)params_postprocess;
  build_blazeface_anchors();
  printf("BlazeFace PP: th=%.2f NMS=%.2f\n", BLZ_SCORE_TH, BLZ_NMS_IOU);
  return 0;
}

int32_t app_postprocess_run(void *pInput[], int nb_input, void *pOutput, void *pInput_param)
{
  (void)pInput_param;

  od_pp_out_t *out = (od_pp_out_t*)pOutput;
  if (!out) return 0;

  /* Expect 4 outputs in this exact order (matches your network.c):
     0: 1x512x16 (reg L), 1: 1x512x1 (score L),
     2: 1x384x1 (score S), 3: 1x384x16 (reg S). */
  if (nb_input < 4 || !pInput[0] || !pInput[1] || !pInput[2] || !pInput[3]) {
    out->pOutBuff = NULL;
    out->nb_detect = 0;
    return 0;
  }

  const float *reg_L = (const float*)pInput[0]; /* 512 x 16 */
  const float *scr_L = (const float*)pInput[1]; /* 512      — already sigmoid()’d */
  const float *scr_S = (const float*)pInput[2]; /* 384      — already sigmoid()’d */
  const float *reg_S = (const float*)pInput[3]; /* 384 x 16 */

  /* Build candidate list (no heap) */
  int nL  = collect_head_into(reg_L, scr_L, BLZ_L_ANCHORS, 0,                s_cand);
  int nS  = collect_head_into(reg_S, scr_S, BLZ_S_ANCHORS, BLZ_L_ANCHORS,    s_cand + nL);
  int tot = nL + nS;

  if (tot <= 0) {
    out->pOutBuff = NULL;
    out->nb_detect = 0;
    return 0;
  }

  /* Greedy NMS across both heads */
  memset(s_suppressed, 0, (size_t)tot);
  int kept = 0;
  for (;;) {
    int best = -1; float bests = -1.f;
    for (int i=0;i<tot;++i)
      if (!s_suppressed[i] && s_cand[i].score > bests) { bests = s_cand[i].score; best = i; }
    if (best < 0) break;

    s_out[kept].x_center    = s_cand[best].cx;
    s_out[kept].y_center    = s_cand[best].cy;
    s_out[kept].width       = s_cand[best].w;
    s_out[kept].height      = s_cand[best].h;
    s_out[kept].conf        = s_cand[best].score;
    s_out[kept].class_index = s_cand[best].class_id;
    kept++;
    s_suppressed[best] = 1;
    if (kept >= BLZ_MAX_OUT) break;

    for (int j=0;j<tot;++j) {
      if (s_suppressed[j]) continue;
      if (iou_xywh(&s_cand[best], &s_cand[j]) > BLZ_NMS_IOU) s_suppressed[j] = 1;
    }
  }

  out->pOutBuff  = s_out;
  out->nb_detect = kept;
  return 0;
}

#endif /* POSTPROCESS_TYPE == POSTPROCESS_CUSTOM */
