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

extern volatile bool arrowUpPressed; // Scroll one page up in chat     
extern volatile bool arrowDownPressed; // Scroll one page down in chat

extern volatile bool arrowRightPressed;
extern volatile bool arrowLeftPressed;                                    

extern volatile bool soundOn; // Turns on sound notification            f1
extern volatile bool soundOff; // Turns off sound notification          f2

extern volatile bool brightnessUpKeyPressed; // Increase screen brightness      Key not known
extern volatile bool brightnessDownKeyPressed; // decrease screen brightness    Key not known


extern volatile bool keyPressDetected; // New flag for single key detection     
extern String lastKeyPressed;            // Last key pressed
extern String message;                 // Shared message buffer

extern volatile bool refresh_Chat_View;
extern volatile bool sound_Switch_Active;

#endif