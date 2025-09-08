#pragma once
#ifdef __cplusplus
extern "C" {
#endif

/* Face detector NN task (moved out of app.c) */
void nn_thread_fct(void *arg);

#ifdef __cplusplus
}
#endif
