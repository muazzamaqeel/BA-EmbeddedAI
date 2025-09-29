/**
 ******************************************************************************
 * @file    app_ui_admin.h
 * @brief   Admin PIN entry screen
 ******************************************************************************
 */

 #ifndef APP_UI_ADMIN_H
 #define APP_UI_ADMIN_H
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /* Result type for Admin screen */
 typedef enum {
     UI_ADMIN_NONE = 0,
     UI_ADMIN_BACK,
     UI_ADMIN_OK
 } UI_AdminResult_t;
 
 /* Functions */
 void UI_AdminScreen_Show(void);
 UI_AdminResult_t UI_AdminScreen_WaitForTouch(void);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* APP_UI_ADMIN_H */
 