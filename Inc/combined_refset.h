#ifndef COMBINED_REFSET_H
#define COMBINED_REFSET_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { const char* name; const float* data; int dim; } EmbRec;

extern EmbRec* g_ref_set;
extern int g_ref_set_count;

void FR_BuildCombinedRefset(void);

#ifdef __cplusplus
}
#endif

#endif /* COMBINED_REFSET_H */
