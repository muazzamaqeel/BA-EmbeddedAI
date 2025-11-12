/* Src/Pipeline_FaceRecognition/refset_compare.c
 * Compare current FaceRec embedding against a compiled-in reference set.
 */
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "ll_aton_runtime.h"
#include "stm32n6xx_hal.h"
#include "cache_utils.h"
#include "npu_guard.h"
/* ---- This typedef MUST match the one in your Generated/embeddings_table.c ---- */
typedef struct { const char* name; const float* data; int dim; } EmbRec;
/* Externs provided by Generated/combined_refset.c */
extern EmbRec* g_ref_set;
extern int     g_ref_set_count;
void FR_BuildCombinedRefset(void);
/* FR output buffer discovery */
extern const LL_Buffer_InfoTypeDef *LL_ATON_Output_Buffers_Info_face_recognition(void);

/* ---------------- utilities ---------------- */
static float cosine_sim(const float *a, const float *b, int n)
{
  /* robust FP accumulation */
  double ab = 0.0, aa = 0.0, bb = 0.0;
  for (int i = 0; i < n; ++i) {
    double x = a[i], y = b[i];
    ab += x * y;  aa += x * x;  bb += y * y;
  }
  if (aa <= 0.0 || bb <= 0.0) return 0.0f;
  double d = ab / (sqrt(aa) * sqrt(bb));
  /* Clamp for numerical safety */
  if (d >  1.0) d =  1.0;
  if (d < -1.0) d = -1.0;
  return (float)d;
}

static int min_int(int a, int b) { return (a < b) ? a : b; }

/* ---------------- public API ---------------- */
void FR_Refset_Summary(void)
{
  printf("[REFSET] entries=%d\r\n", g_ref_set_count);
  for (int i = 0; i < g_ref_set_count; ++i) {
    const char *nm = (g_ref_set[i].name && g_ref_set[i].name[0]) ? g_ref_set[i].name : "(unnamed)";
    printf("  - #%d name=%s dim=%d\r\n", i, nm, g_ref_set[i].dim);
  }
}

/* Compare current FaceRec output (probe) against the compiled ref set. */
void FR_CompareRefset_FromCurrentOut(void)
{
  if (g_ref_set_count <= 0) {
    printf("[REFSET][ERR] No reference embeddings linked.\r\n");
    return;
  }

  const LL_Buffer_InfoTypeDef *out_info = LL_ATON_Output_Buffers_Info_face_recognition();
  if (!out_info) {
    printf("[REFSET][ERR] FaceRec output buffer info not available.\r\n");
    return;
  }

  void   *out_ptr = LL_Buffer_addr_start(&out_info[0]);
  size_t  out_len = (size_t)LL_Buffer_len(&out_info[0]);
  if (!out_ptr || out_len < sizeof(float)) {
    printf("[REFSET][ERR] FaceRec output buffer invalid (len=%lu).\r\n", (unsigned long)out_len);
    return;
  }

  /* Make sure CPU sees what NPU wrote */
  DCACHE_Invalidate(out_ptr, out_len);

  const float *probe = (const float *)out_ptr;
  const int    probe_dim = (int)(out_len / sizeof(float));

  /* Scan all references; keep top-3 */
  int   best_i = -1, second_i = -1, third_i = -1;
  float best_s = -2.0f, second_s = -2.0f, third_s = -2.0f;

  for (int i = 0; i < g_ref_set_count; ++i) {
    const int dim = min_int(probe_dim, g_ref_set[i].dim);
    if (dim <= 0) continue;
    float s = cosine_sim(probe, g_ref_set[i].data, dim);
    if (s > best_s) {
      third_s = second_s; third_i = second_i;
      second_s = best_s;  second_i = best_i;
      best_s = s;         best_i  = i;
    } else if (s > second_s) {
      third_s = second_s; third_i = second_i;
      second_s = s;       second_i = i;
    } else if (s > third_s) {
      third_s = s;        third_i = i;
    }
  }
  printf("[REFSET] probe_dim=%d  refs=%d\r\n", probe_dim, g_ref_set_count);
  if (best_i < 0) {
    printf("[REFSET][WARN] No valid comparisons performed.\r\n");
    return;
  }
  const char *n1 = (g_ref_set[best_i].name   && g_ref_set[best_i].name[0])   ? g_ref_set[best_i].name   : "(unnamed)";
  const char *n2 = (second_i >= 0 && g_ref_set[second_i].name && g_ref_set[second_i].name[0]) ? g_ref_set[second_i].name : "(none)";
  const char *n3 = (third_i  >= 0 && g_ref_set[third_i].name  && g_ref_set[third_i].name[0])  ? g_ref_set[third_i].name  : "(none)";
  printf("[REFSET] TOP-1: idx=%d name=%s  cos=%.4f\r\n", best_i,   n1, best_s);
  if (second_i >= 0) printf("[REFSET] TOP-2: idx=%d name=%s  cos=%.4f\r\n", second_i, n2, second_s);
  if (third_i  >= 0) printf("[REFSET] TOP-3: idx=%d name=%s  cos=%.4f\r\n", third_i,  n3, third_s);
}
