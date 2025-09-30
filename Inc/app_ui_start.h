/**
 ******************************************************************************
 * @file    app_ui_start.h
 * @brief   Start screen (UI) header
 ******************************************************************************
 */

 #ifndef APP_UI_START_H
 #define APP_UI_START_H
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 #include <stdint.h>
 
 /* ===== Button results ===== */
 typedef enum {
     UI_BTN_START = 0,
     UI_BTN_ADMIN,   /* keep for compatibility if some code still references it */
     UI_BTN_PIN      /* new: left button = PIN */
 } UI_ButtonResult_t;
 
 /* ===== Public functions ===== */
 void UI_StartScreen_Show(void);
 UI_ButtonResult_t UI_WaitForButton(void);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* APP_UI_START_H */
 