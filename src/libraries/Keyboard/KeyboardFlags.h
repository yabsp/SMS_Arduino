#ifndef KEYBOARD_FLAGS_H
#define KEYBOARD_FLAGS_H

#include <Arduino.h>

extern volatile bool enterKeyPressed;
extern volatile bool deleteKeyPressed;
extern volatile bool keyPressDetected; // New flag for single key detection
extern char lastKeyPressed;            // Last key pressed
extern char message[];                 // Shared message buffer

#endif