/**
 ******************************************************************************
 * @file    app_ui_pin_face_rec.h
 * @brief   Public API for FaceRec PIN entry screen
 ******************************************************************************
 */

 #ifndef APP_UI_PIN_FACE_REC_H
 #define APP_UI_PIN_FACE_REC_H
 
 #include <stdbool.h>
 
void UI_FR_PinScreen_Show(void);
void UI_FR_PinScreen_WaitForOK(void);
void FR_UI_ShowStatus(const char *msg, uint32_t color);
void FR_UI_DrawPinBuffer(int pin_len);
 
#endif /* APP_UI_PIN_FACE_REC_H */
