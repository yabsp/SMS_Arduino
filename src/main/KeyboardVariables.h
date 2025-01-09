#ifndef KEYBOARDVARIABLES_H
#define KEYBOARDVARIABLES_H

#define MAX_MESSAGE_LENGTH 2048
#include <stdint.h>

/*
bool altgrActive = false;
bool shiftActive = false;
bool capsActive = false;

extern unsigned long shiftLastTime = 0;
extern unsigned long altgrLastTime = 0;

extern const unsigned long timeout = 200; // Timeout for Shift and AltGR in ms
extern unsigned long currentTime;

extern uint16_t scanval = 0;
extern uint8_t lastscan = 0;
extern uint8_t lastlastscan = 0;
extern char input = NULL;
*/

extern bool altgrActive;
extern bool shiftActive;
extern bool capsActive;

extern unsigned long shiftLastTime;
extern unsigned long altgrLastTime;

extern const unsigned long timeout; // Timeout for Shift and AltGR in ms
extern unsigned long currentTime;

extern uint16_t scanval;
extern uint8_t lastscan;
extern uint8_t lastlastscan;
extern char input;

#endif