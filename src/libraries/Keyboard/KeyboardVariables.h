#ifndef KEYBOARDVARIABLES_H
#define KEYBOARDVARIABLES_H

#include <LiquidCrystal.h>

#define MAX_MESSAGE_LENGTH 1024

bool altgrActive = false;
bool shiftActive = false;
bool capsActive = false;

unsigned long shiftLastTime = 0;
unsigned long altgrLastTime = 0;

const unsigned long timeout = 100; // Timeout for Shift and AltGR in ms

uint8_t lastscan = 0;
uint8_t lastlastscan = 0;
uint8_t line = 0, col = 0;
char input = NULL;

#endif