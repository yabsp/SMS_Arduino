#ifndef CONFIG_H
#define CONFIG_H

#include <MCUFRIEND_kbv.h> 

// Constants for touchscreen calibration & SD-card -------------------------------------
const int XP = 8, XM = A2, YP = A3, YM = 9; // Pin assignments
const int TS_LEFT = 27, TS_RT = 907, TS_TOP = 101, TS_BOT = 932; // Display calibration
const int chipSelect = 53;


// Declare shared variables ------------------------------------------------------------
extern MCUFRIEND_kbv tft;            // Declare TFT object
extern int Cursor_X, Cursor_Y;       // Cursor position
extern bool Cursor_Pressed;          // Indicates if the touchscreen is pressed
extern uint8_t Current_Menu;         // Declare the global Current_Menu variable
extern int activeInput;              // Active Input (0: none, 1: phoneNumberInput, 2: contactInput, 3: messageInput)


// Touchscreen sensitivity -------------------------------------------------------------
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Chat constraints --------------------------------------------------------------------
#define MAX_NAME_LENGTH 20
//#define MAX_MESSAGES 1
#define MAX_MESSAGE_LENGTH 30
#define MAX_CHATS 10
#define MAX_INPUT_LENGTH 64


// Screen dimensions -------------------------------------------------------------------
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 320


// Colors ------------------------------------------------------------------------------
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define TURQUOISE   0x4639
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF

#endif