#ifndef KEYBOARD_FLAGS_H
#define KEYBOARD_FLAGS_H

#include <Arduino.h>

// Flags for functionality
extern volatile bool keyboardActive;

// Flags for special keys
extern volatile bool enterKeyPressed; // Send SMS when in chat window
extern volatile bool deleteKeyPressed; // Delete last typed character
extern volatile bool escKeyPressed; // Go back to last scene?
extern volatile bool tabKeyPressed; // ?                                        Key might not work
extern volatile bool numLockKeyPressed; // ?      

extern volatile bool arrowUpPressed; // Scroll one page up in chat              Key missing
extern volatile bool arrowDownPressed; // Scroll one page down in chat          Key missing
extern volatile bool arrowRightPressed; // ?                                    Key missing
extern volatile bool arrowLeftPressed; // ?                                     Key missing

extern volatile bool volumeUpKeyPressed; // Increase speaker volume             Key not known
extern volatile bool volumeDownKeyPressed; // decrease speaker volume           Key not known

extern volatile bool brightnessUpKeyPressed; // Increase screen brightness      Key not known
extern volatile bool brightnessDownKeyPressed; // decrease screen brightness    Key not known

extern volatile bool keyPressDetected; // New flag for single key detection     
extern char lastKeyPressed;            // Last key pressed
extern char message[];                 // Shared message buffer

#endif