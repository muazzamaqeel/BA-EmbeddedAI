#include <string.h>
#include <stdbool.h>

#include "app_change_pin.h"   // <-- REQUIRED for CP_State and CP_Result
#include "admin_pin.h"        // optional if using admin pin decryption later


char g_current_pin[8] = "1234";
char g_admin_pin[8] = "1234";
char g_decrypted_pin[8] = {0};

#define CP_KEY_W   90
#define CP_KEY_H   65
#define CP_KEY_SP  20
#define CP_KEYPAD_ORIGIN_X  ((800/2 - (3*CP_KEY_W + 2*CP_KEY_SP)/2))
#define CP_KEYPAD_ORIGIN_Y  100

#define PIN_BOX_X   180
#define PIN_BOX_Y    20
#define PIN_BOX_W   440
#define PIN_BOX_H    50

char cp_pin_buffer[8];
int  cp_pin_len = 0;
char new_pin[8];
CP_State cp_state;
