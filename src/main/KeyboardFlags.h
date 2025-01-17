#ifndef KEYBOARD_FLAGS_H
#define KEYBOARD_FLAGS_H

#include <Arduino.h>

// Flags for functionality
extern volatile bool keyboardActive;

// Flags for special keys
extern volatile bool enterKeyPressed_SMS; // Send SMS when in chat window
extern volatile bool enterKeyPressed_Screen;
extern volatile bool deleteKeyPressed; // Delete last typed character
extern volatile bool escKeyPressed; // Go back to last scene?
extern volatile bool tabKeyPressed; // ?                                        
extern volatile bool numLockKeyPressed; // ?      

extern volatile bool arrowUpPressed; // Scroll one page up in chat              Not Implemented      
extern volatile bool arrowDownPressed; // Scroll one page down in chat          Not Implemented 
extern volatile bool arrowRightPressed; // ?                                    Not Implemented 
extern volatile bool arrowLeftPressed; // ?                                     Not Implemented 

extern volatile bool volumeUpKeyPressed; // Increase speaker volume             Key not known
extern volatile bool volumeDownKeyPressed; // decrease speaker volume           Key not known

extern volatile bool brightnessUpKeyPressed; // Increase screen brightness      Key not known
extern volatile bool brightnessDownKeyPressed; // decrease screen brightness    Key not known

extern volatile bool keyPressDetected; // New flag for single key detection     
extern String lastKeyPressed;            // Last key pressed
extern String message;                 // Shared message buffer

#endif