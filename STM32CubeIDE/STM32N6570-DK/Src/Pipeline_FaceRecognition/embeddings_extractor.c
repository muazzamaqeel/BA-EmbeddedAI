/**
 * @file embeddings_extractor.c
 * @brief Boot-time embeddings from existing BMP files (auto-embedded).
 *
 * - Reads g_user_bmps[] (auto-generated from UserFace/*.bmp)
 * - Supports 24-bit uncompressed BMP (BI_RGB), top-down or bottom-up
 * - Center-square resamples → FaceRec input (FR_IN_W x FR_IN_H), u8 quantized
 * - Runs FaceRec once per image and prints:  <label>|<dim>|v0,...,vN-1  (floats after dequant)
 * - No live-frame logic, no camera, no detector usage
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "stm32n6xx_hal.h"

#include "emb_assets.h"              /* g_user_bmps[], g_user_bmps_count */
#include "face_recognition.h"        /* fr_init(), fr_set_subject(), FR_IN_W/H */
#include "ll_aton_runtime.h"         /* LL_Buffer_InfoTypeDef, LL_Buffer_* */
#include "cache_utils.h"             /* DCACHE_Clean/Invalidate, dcache_align_range */
#include "npu_guard.h"               /* NPU_Lock/Unlock, TAG_FR */

/* ----------- Defaults if not provided ----------- */
#ifndef FR_IN_W
#define FR_IN_W    160   /* INT8 FaceRec input width */
#endif
#ifndef FR_IN_H
#define FR_IN_H    160   /* INT8 FaceRec input height */
#endif
#ifndef FR_EMB_SIZE
#define FR_EMB_SIZE 512  /* INT8 FaceRec embedding dimension */
#endif
#ifndef FR_SUBJECT_NAME
#define FR_SUBJECT_NAME "ME"
#endif

/* FaceRec runtime entry points for your FR instance (provided by your app) */
extern void FaceRec_Run_NoLock(void);
extern const LL_Buffer_InfoTypeDef *LL_ATON_Input_Buffers_Info_face_recognition(void);
extern const LL_Buffer_InfoTypeDef *LL_ATON_Output_Buffers_Info_face_recognition(void);

/* ============================== */
/*     Minimal BMP 24-bit I/O     */
/* ============================== */

typedef struct {
  const uint8_t *pixels;   /* pointer to start of pixel array                */
  int width;               /* image width                                    */
  int height_abs;          /* absolute height                                */
  int row_stride;          /* bytes per row incl. 4B padding                 */
  int bpp;                 /* bits per pixel (expect 24)                     */
  int compression;         /* expect 0 (BI_RGB)                              */
  bool top_down;           /* true if height < 0 (origin at top-left)        */
} Bmp24View;

static uint32_t rd_le32(const uint8_t *p){ return (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24); }
static uint16_t rd_le16(const uint8_t *p){ return (uint16_t)p[0] | ((uint16_t)p[1]<<8); }

static int bmp24_open(const uint8_t *buf, uint32_t len, Bmp24View *out)
{
  if (!buf || len < 54) return -1;
  if (buf[0] != 'B' || buf[1] != 'M') return -2;

  uint32_t pixel_off = rd_le32(buf + 10);
  const uint8_t *dib = buf + 14;
  if (len < 14 + 40) return -3;            /* need BITMAPINFOHEADER (40 bytes) */

  /* Parse BITMAPINFOHEADER (common path) */
  uint32_t dib_size  = rd_le32(dib + 0);   (void)dib_size;
  int32_t  w         = (int32_t) rd_le32(dib + 4);
  int32_t  h_signed  = (int32_t) rd_le32(dib + 8);
  uint16_t planes    = rd_le16(dib + 12);
  uint16_t bpp       = rd_le16(dib + 14);
  uint32_t comp      = rd_le32(dib + 16);

  if (planes != 1)           return -4;
  if (bpp != 24)             return -5;    /* only 24-bit supported */
  if (comp != 0 /*BI_RGB*/)  return -6;    /* only uncompressed     */
  if (w <= 0 || h_signed == 0) return -7;

  bool top_down   = (h_signed < 0);
  int  h_abs      = top_down ? -h_signed : h_signed;
  int  row_stride = ((w * 3 + 3) / 4) * 4;

  if (pixel_off + (uint32_t)row_stride * (uint32_t)h_abs > len) return -8;

  out->pixels     = buf + pixel_off;
  out->width      = w;
  out->height_abs = h_abs;
  out->row_stride = row_stride;
  out->bpp        = 24;
  out->compression= 0;
  out->top_down   = top_down;
  return 0;
}


/* Fetch B,G,R from integer pixel coords (clamped) → return RGB floats [0..255] */
static inline void bmp24_get_bgr(const Bmp24View *v, int x, int y, float *r, float *g, float *b)
{
  if (x < 0) x = 0; if (x >= v->width) x = v->width - 1;
  if (y < 0) y = 0; if (y >= v->height_abs) y = v->height_abs - 1;

  int row = v->top_down ? y : (v->height_abs - 1 - y);
  const uint8_t *p = v->pixels + row * v->row_stride + x * 3;
  float B = (float)p[0], G = (float)p[1], R = (float)p[2];
  *r = R; *g = G; *b = B;   /* convert to RGB order */
}

/* Center-square bilinear resample BMP24 → FR input NHWC uint8 (quantized [-1..1] → u8) */
static void bmp24_center_square_to_fr_input_u8q(const Bmp24View *v, uint8_t *dst, int dw, int dh)
{
  /* centered square crop */
  int side = (v->width < v->height_abs) ? v->width : v->height_abs;
  int x0i = (v->width  - side) / 2;
  int y0i = (v->height_abs - side) / 2;

  const float sx = (float)side / (float)dw;
  const float sy = (float)side / (float)dh;

  /* Quantization chosen to match firmware & TFLite INT8 pipeline:
     float in [-1,1]  →  q = round(x/scale + zp), with scale = 2/255, zp = 128. */
  const float inv_in_scale = 127.5f; /* = 1 / (2/255) */
  const float zp = 128.0f;

  for (int y=0; y<dh; ++y) {
    float fy = y0i + (y + 0.5f) * sy - 0.5f;
    int y0 = (int)floorf(fy), y1 = y0 + 1;
    float wy = fy - (float)y0;

    /* clamp for safe indexing */
    if (y0 < 0) y0 = 0; if (y0 >= v->height_abs) y0 = v->height_abs - 1;
    if (y1 < 0) y1 = 0; if (y1 >= v->height_abs) y1 = v->height_abs - 1;

    for (int x=0; x<dw; ++x) {
      float fx = x0i + (x + 0.5f) * sx - 0.5f;
      int x0 = (int)floorf(fx), x1 = x0 + 1;
      float wx = fx - (float)x0;

      if (x0 < 0) x0 = 0; if (x0 >= v->width) x0 = v->width - 1;
      if (x1 < 0) x1 = 0; if (x1 >= v->width) x1 = v->width - 1;

      float r00,g00,b00, r01,g01,b01, r10,g10,b10, r11,g11,b11;
      bmp24_get_bgr(v, x0, y0, &r00,&g00,&b00);
      bmp24_get_bgr(v, x1, y0, &r01,&g01,&b01);
      bmp24_get_bgr(v, x0, y1, &r10,&g10,&b10);
      bmp24_get_bgr(v, x1, y1, &r11,&g11,&b11);

      float w00=(1.f-wx)*(1.f-wy), w01=wx*(1.f-wy),
            w10=(1.f-wx)*wy      , w11=wx*wy;

      /* RGB in [0..255] after bilinear */
      float r255 = r00*w00 + r01*w01 + r10*w10 + r11*w11;
      float g255 = g00*w00 + g01*w01 + g10*w10 + g11*w11;
      float b255 = b00*w00 + b01*w01 + b10*w10 + b11*w11;

      /* Map [0..255] → [-1,1] */
      float rq = r255 * (1.0f/127.5f) - 1.0f;
      float gq = g255 * (1.0f/127.5f) - 1.0f;
      float bq = b255 * (1.0f/127.5f) - 1.0f;

      /* Quantize: q_u8 = round(x/scale + zp), scale=2/255, zp=128 */
      int d = (y*dw + x)*3;
      int qr = (int)lrintf(rq * inv_in_scale + zp); if (qr < 0) qr = 0; if (qr > 255) qr = 255;
      int qg = (int)lrintf(gq * inv_in_scale + zp); if (qg < 0) qg = 0; if (qg > 255) qg = 255;
      int qb = (int)lrintf(bq * inv_in_scale + zp); if (qb < 0) qb = 0; if (qb > 255) qb = 255;

      dst[d+0] = (uint8_t)qr;
      dst[d+1] = (uint8_t)qg;
      dst[d+2] = (uint8_t)qb;
    }
  }
}

/* ============================== */
/*       FR init helper           */
/* ============================== */

static void fr_init_once(void)
{
  static int inited = 0;
  if (!inited) {
    fr_init();
    fr_set_subject(FR_SUBJECT_NAME);
    fr_set_match_threshold(0.80f);
    inited = 1;
  }
}

/* ============================== */
/*      Public entry point        */
/* ============================== */
/* Call this early at boot to dump embeddings from embedded BMP assets. */
int FR_ExtractEmbeddings_FromAssets(void)
{
  fr_init_once();

  /* Early out if no assets (still print an empty header for parsers) */
  if (g_user_bmps_count <= 0) {
    printf("# data_embeddings (assets)\r\n# count=0\r\n");
    return 0;
  }

  /* Get FaceRec IO buffers */
  const LL_Buffer_InfoTypeDef *fr_in_info  = LL_ATON_Input_Buffers_Info_face_recognition();
  const LL_Buffer_InfoTypeDef *fr_out_info = LL_ATON_Output_Buffers_Info_face_recognition();

  /* INT8 pipeline: input=uint8, output=int8 */
  uint8_t *fr_in      = (uint8_t*)LL_Buffer_addr_start(&fr_in_info[0]);
  int8_t  *fr_out     = (int8_t *)LL_Buffer_addr_start(&fr_out_info[0]);
  uint32_t fr_in_len  = LL_Buffer_len(&fr_in_info[0]);
  uint32_t fr_out_len = LL_Buffer_len(&fr_out_info[0]);

  const uint32_t need_in_bytes = (uint32_t)(FR_IN_W * FR_IN_H * 3);  /* 160*160*3 = 76800 */

  if (fr_in_len < need_in_bytes) {
    printf("[FR][BOOT][ERR] FR input buffer too small (%lu < %lu)\r\n",
           (unsigned long)fr_in_len, (unsigned long)need_in_bytes);
    printf("# data_embeddings (assets)\r\n# count=0\r\n");
    return -1;
  }
  if (fr_out_len < FR_EMB_SIZE) {
    printf("[FR][BOOT][ERR] FR output buffer too small (%lu < %u)\r\n",
           (unsigned long)fr_out_len, (unsigned)FR_EMB_SIZE);
    printf("# data_embeddings (assets)\r\n# count=0\r\n");
    return -2;
  }

#if defined(USE_DCACHE)
  { void *o=fr_out; size_t l=fr_out_len; dcache_align_range(&o,&l); SCB_CleanInvalidateDCache_by_Addr(o,(int)l); }
#endif

  printf("# data_embeddings (assets)\r\n# count=%d\r\n", g_user_bmps_count);

  for (int i=0; i<g_user_bmps_count; ++i) {
    const EmbeddedBMP *E = &g_user_bmps[i];

    /* Decode BMP header */
    Bmp24View v;
    int st = bmp24_open(E->data, E->size, &v);
    if (st != 0) {
      printf("[FR][BOOT][WARN] %s: unsupported BMP (err=%d), skipping.\r\n",
             E->name ? E->name : "img", st);
      continue;
    }

    /* Prepare FR input from BMP (center-square → 160x160, u8 quantized) */
    bmp24_center_square_to_fr_input_u8q(&v, fr_in, FR_IN_W, FR_IN_H);

    /* CPU -> NPU */
    DCACHE_Clean(fr_in, need_in_bytes);

    /* Run FaceRec once */
    NPU_Lock(TAG_FR);
    uint32_t t0 = HAL_GetTick();
    FaceRec_Run_NoLock();
    uint32_t dt = HAL_GetTick() - t0;
    NPU_Unlock(TAG_FR);

    /* NPU -> CPU */
#if defined(USE_DCACHE)
    { void *o=fr_out; size_t l=fr_out_len; dcache_align_range(&o,&l); SCB_InvalidateDCache_by_Addr(o,(int)l); }
#else
    DCACHE_Invalidate(fr_out, fr_out_len);
#endif

    /* Output is INT8; dequantize with scale=1/128, zp=0 for printing */
    int emb_dim = (int)((fr_out_len < FR_EMB_SIZE) ? fr_out_len : FR_EMB_SIZE);
    float vec_f[FR_EMB_SIZE];
    for (int k=0; k<emb_dim; ++k) {
      vec_f[k] = (float)fr_out[k] * 0.0078125f;  /* 1/128 */
    }

    /* Print one line per image:  <label>|<dim>|v0,...,vN-1 */
    const char *label = (E->name && E->name[0]) ? E->name : "img";
    printf("%s|%d|", label, emb_dim);
    for (int k=0; k<emb_dim; ++k) {
      printf((k+1<emb_dim) ? "%.6f," : "%.6f", vec_f[k]);
    }
    printf("\r\n");

    printf("[FR][BOOT] %s ok (%dx%d → %dx%d) in %lums\r\n",
           label, v.width, v.height_abs, FR_IN_W, FR_IN_H, (unsigned long)dt);
  }

  printf("[FR][BOOT] Embedding extraction finished.\r\n");
  return 0;
}
