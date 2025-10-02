/**
 ******************************************************************************
 * @file    app_ui_pin_face_rec.h
 * @brief   Public API for FaceRec PIN entry screen
 ******************************************************************************
 */

 #ifndef APP_UI_PIN_FACE_REC_H
 #define APP_UI_PIN_FACE_REC_H
 
 #include <stdbool.h>
 
 /**
  * @brief Show the FaceRec PIN entry screen (with visible keypad)
  *        - Disables sleep while active
  *        - Draws background, keypad, and masked PIN buffer
  */
 void UI_FR_PinScreen_Show(void);
 
 /**
  * @brief Wait until user presses "OK" and validates PIN
  *        - Blocks until correct PIN is entered
  *        - Re-enables sleep on success
  */
 void UI_FR_PinScreen_WaitForOK(void);
 
 #endif /* APP_UI_PIN_FACE_REC_H */
 