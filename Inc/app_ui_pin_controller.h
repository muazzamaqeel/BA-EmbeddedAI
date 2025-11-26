#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern char pin_buffer[8];
extern int  pin_len;

void UI_PinScreen_WaitForOK_Controller(void);

#ifdef __cplusplus
}
#endif
