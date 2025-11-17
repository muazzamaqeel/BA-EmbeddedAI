#pragma once
#ifdef __cplusplus
extern "C" {
#endif

void UI_FSM_Init(void);
/* Blocking loop that returns after pipeline launches */
void UI_FSM_Run(void);

#ifdef __cplusplus
}
#endif
