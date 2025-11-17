#pragma once
#ifdef __cplusplus
extern "C" {
#endif

/* exported FaceRec PP thread entry */
void pp_thread_fct(void *arg);
void FR_ResetRecognitionState(void);

#ifdef __cplusplus
}
#endif
