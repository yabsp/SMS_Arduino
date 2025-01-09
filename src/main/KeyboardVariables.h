#ifndef KEYBOARDVARIABLES_H
#define KEYBOARDVARIABLES_H

#define MAX_MESSAGE_LENGTH 2048
#include <stdint.h>

extern bool altgrActive;
extern bool shiftActive;
extern bool capsActive;

extern unsigned long shiftLastTime;
extern unsigned long altgrLastTime;

extern const unsigned long timeout;
extern unsigned long currentTime;

extern uint16_t scanval;
extern uint8_t lastscan;
extern uint8_t lastlastscan;
extern char input;

#endif