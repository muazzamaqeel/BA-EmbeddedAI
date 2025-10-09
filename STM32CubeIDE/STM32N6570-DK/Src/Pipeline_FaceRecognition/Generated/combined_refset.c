/* Auto-generated merged reference set — DO NOT EDIT */
#include <stddef.h>
#include <stdint.h>

#ifndef EMBREC_DEFINED
#define EMBREC_DEFINED
typedef struct { const char* name; const uint8_t* data; int dim; } EmbRec;
#endif

#include "embeddings_table_Aida.c"
#include "embeddings_table_Keti.c"
#include "embeddings_table_Muazzam.c"

const EmbRec* g_all_ref_sets[] = {
    g_ref_set_Aida,
    g_ref_set_Keti,
    g_ref_set_Muazzam,
};

const int g_all_ref_set_counts[] = {
    g_ref_set_count_Aida,
    g_ref_set_count_Keti,
    g_ref_set_count_Muazzam,
};

static EmbRec g_ref_set_combined[512];
static int g_ref_set_combined_count = 0;
EmbRec* g_ref_set = g_ref_set_combined;
int g_ref_set_count = 0;

void FR_BuildCombinedRefset(void) {
    g_ref_set_combined_count = 0;
    g_ref_set_count = 0;
    int total = sizeof(g_all_ref_set_counts)/sizeof(int);
    for (int i = 0; i < total; i++) {
        int cnt = g_all_ref_set_counts[i];
        const EmbRec* sub = g_all_ref_sets[i];
        for (int j = 0; j < cnt && g_ref_set_combined_count < 512; j++) {
            g_ref_set_combined[g_ref_set_combined_count++] = sub[j];
        }
    }
    g_ref_set = g_ref_set_combined;
    g_ref_set_count = g_ref_set_combined_count;
}
