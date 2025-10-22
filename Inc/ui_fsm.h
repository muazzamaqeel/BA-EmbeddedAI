/**
 ******************************************************************************
 * @file    ui_fsm.h
 * @brief   Finite State Machine controller for all UI screens
 ******************************************************************************
 */

 #ifndef UI_FSM_H
 #define UI_FSM_H
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /* ---------- UI State Types ---------- */
 typedef enum {
     UI_STATE_START,        /* Home screen */
     UI_STATE_PIN,          /* Admin PIN entry */
     UI_STATE_ADMIN,        /* Admin screen */
     UI_STATE_CHANGE_PIN,   /* Change PIN flow */
     UI_STATE_FACE_REC      /* Face recognition / main app */
 } UI_State;
 
 /* ---------- API ---------- */
 void UI_FSM_Init(void);
 void UI_FSM_Run(void);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* UI_FSM_H */
 