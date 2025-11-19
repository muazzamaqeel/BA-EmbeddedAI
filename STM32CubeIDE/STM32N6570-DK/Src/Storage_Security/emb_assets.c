#include "emb_assets.h"

/* Example: one dummy BMP — replace with real auto-generated content */
static const uint8_t dummy_bmp[] = { 0x42,0x4D /*...*/ };

const EmbeddedBMP g_user_bmps[] = {
    { "dummy.bmp", dummy_bmp, sizeof(dummy_bmp) }
};
const int g_user_bmps_count = sizeof(g_user_bmps) / sizeof(g_user_bmps[0]);
