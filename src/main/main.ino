/*
#define CLOCK 7 //D-
#define DATA 6  //D+
*/


#define CLOCK 51  //D-
#define DATA 50   //D+


/*
#define CLOCK 69  //D-
#define DATA 68   //D+
*/

/*
#define CLOCK 19    //D-
#define DATA 18     //D+
*/

#include "Keymaps.h"
#include "KeyboardFlags.h"
#include "KeyboardVariables.h"
#include "KeyboardFunctions.h"
#include "sim7600g-h.h"
/*
#include "Utils.h"
#include "ChatHandler.h"
#include "Config.h"
#include "Menu.h"
#include "TouchHandler.h"

*/
#include "display.h"

extern volatile bool keyboardActive = true;

extern volatile bool enterKeyPressed = false; // Send SMS when in chat window
extern volatile bool deleteKeyPressed = false; // Delete last typed character
extern volatile bool escKeyPressed = false; // Go back to last scene?
extern volatile bool tabKeyPressed = false; // ?
extern volatile bool numLockKeyPressed = false; // ?

extern volatile bool arrowUpPressed = false; // Scroll one page up in chat
extern volatile bool arrowDownPressed = false; // Scroll one page down in chat
extern volatile bool arrowRightPressed = false; // ?
extern volatile bool arrowLeftPressed = false; // ?

extern volatile bool volumeUpKeyPressed = false; // Increase speaker volume
extern volatile bool volumeDownKeyPressed = false; // decrease speaker volume;

extern volatile bool brightnessUpKeyPressed = false; // Increase screen brightness
extern volatile bool brightnessDownKeyPressed = false; // decrease screen brightness

volatile bool keyPressDetected = false;
char lastKeyPressed = '\0'; 
String message; // Message buffer

void setup()
{
  Serial.begin(115200);
  setupSim7600();
  setupDisplay();
  pinMode(CLOCK, INPUT_PULLUP); //For most keyboards the builtin pullups are sufficient, so the 10k pullups can be omitted
  pinMode(DATA, INPUT_PULLUP);

  /*
  bitSet(PCICR, PCIE0); // Enable pin change interrupts on pin D0-D7
  bitSet(PCMSK0, (1 << (CLOCK - 50)));
  */
  
  /*
  bitSet(PCICR, PCIE2); // Enable pin change interrupts on pin D0-D7
  bitSet(PCMSK2, CLOCK); // Pin change interrupt on Clock pin
  */

  /*
  bitSet(PCICR, PCIE2);
  bitSet(PCMSK2, PCINT23);
  */

  bitSet(PCICR, PCIE0); 
  bitSet(PCMSK0, PCINT3); 
  
 }

ISR(PCINT0_vect)
//ISR(PCINT2_vect)
{
  if(keyboardActive) {
    scanval = 0;
    for(int i = 0; i<11; i++)
    {
      while(digitalRead(CLOCK));
      scanval |= digitalRead(DATA) << i; // 1 start, 8 data bits, 1 parity, 1 stop bit
      while(!digitalRead(CLOCK));
    }

    scanval >>= 1; // ignore the start bit
    scanval &= 0xFF; // ignore the parity and stop bit, isolate 8 data bits
    //handle_key_press(scanval, lastscan, lastlastscan);

    //Serial.println(scanval, HEX);
    
    if(lastscan != 0xF0 && scanval != 0xF0){
      if (scanval == 0x12 || scanval == 0x59) { // Shift press
        shiftLastTime = millis(); // Reset Shift timer
        shiftActive = true;
      } else if (scanval == 0x11 && lastscan == 0xE0) { // AltGR press
        altgrActive = true;
        altgrLastTime = millis();
      } else if(scanval == 0x58){
        capsActive = !capsActive;
      }

      switch(scanval) // Cases for speccial keys
      {
      case 0x5A: //Enter
        Serial.println("In 0x5A case");
        enterKeyPressed = true;
        handle_enter_key();
        Serial.println("ENTER_FOR_NEW_LINE");
        break;
      case 0x66: //Backspace
        if (message.length() > 0) {
          deleteKeyPressed = true;
          delete_last_char_from_message();
          Serial.println("DELETE_LAST_CHAR_ON_LCD_SCREEN");
        }
      break;
      
      case 0x76: //ESC
        if (message.length() > 0) {
          escKeyPressed = true;

        }
      break;

      case 0x0D: //TAB
        if (message.length() > 0) {
          tabKeyPressed = true;

        }
      break;

      case 0x77: //NUMLOCK
        if (message.length() > 0) {
          numLockKeyPressed = true;

        }
      break;
      
      case 0x75: //ARROW UP
        if (message.length() > 0 && lastscan == 0xE0) {
          arrowUpPressed = true;

        }
      break;
      
      case 0x72: //ARROW DOWN
        if (message.length() > 0 && lastscan == 0xE0) {
          arrowDownPressed = true;

        }
      break;

      case 0x74: //ARROW RIGHT
        if (message.length() > 0 && lastscan == 0xE0) {
          arrowRightPressed = true;

        }
      break;

      case 0x6B: //ARROW LEFT
        if (message.length() > 0 && lastscan == 0xE0) {
          arrowLeftPressed = true;

        }
      break;
      /*
      case 0x66: //FX Volume up
        if (message.length() > 0) {
          volumeUpKeyPressed = true;

        }
      break;

      case 0x66: //FX Volume down
        if (message.length() > 0) {
          volumeDownKeyPressed = true;

        }
      break;

      case 0x66: //FX Brightness up
        if (message.length() > 0) {
          brightnessUpKeyPressed = true;

        }
      break;

      case 0x66: //FX Brightness down
        if (message.length() > 0) {
          brightnessDownKeyPressed = true;
        }
      break;
      */

     case 0x4C: //ö

        if (shiftActive) {
          add_char_to_message("O");
          add_char_to_message("e");

          lastKeyPressed = "Oe";
        } else {
          add_char_to_message("o");
          add_char_to_message("e");

          lastKeyPressed = "oe";
        }

      break;

      case 0x54: //ü

        if (shiftActive) {
          add_char_to_message("U");
          add_char_to_message("e");

          lastKeyPressed = "Ue";
        } else {
          add_char_to_message("u");
          add_char_to_message("e");

          lastKeyPressed = "ue";
        }
      break;

      case 0x52: //ä

        if (shiftActive) {
          add_char_to_message("A");
          add_char_to_message("e");

          lastKeyPressed = "Ae";
          } else {
          add_char_to_message("a");
          add_char_to_message("e");

          lastKeyPressed = "ae";
        }
      break;
      
      case 0x4E: //ß

      if (shiftActive) {
        add_char_to_message("?");

        lastKeyPressed = "?";
      } else if (altgrActive) {
        add_char_to_message("\\");

        lastKeyPressed = "\\";
      } else {
        add_char_to_message("s");
        add_char_to_message("s");

        lastKeyPressed = "ss";
      }
      break;

      default:
        if (shiftActive) {
          input = keymapGERShift[scanval]; // input = shift + key
        } else if (altgrActive) {
          input = keymapGERAltGR[scanval]; // input = AltGR + key
        } else if (capsActive) {
          input = keymapGERShift[scanval]; 
        }else {
          if (scanval == 0x58) {
            input = 0;
          } else {
            input = keymapGER[scanval];      // input = key
          }
        }
        if (input != 0) { // only print if valid key is pressed
          if (scanval == 0x58) {
            input = 0;
          } else {
            add_char_to_message(input);
            Serial.println(input);

            lastKeyPressed = input;
            keyPressDetected = true;
          }
        }
      }
    } 
    
    lastlastscan = lastscan;
    lastscan = scanval;
  }

  //bitSet(PCIFR, PCIF2);
  //PCIFR |= PCIF0;
  bitSet(PCIFR, PCIF0);
}

void loop()
{
  loopSim7600();
  loopDisplay();
  currentTime = millis();

  // Deactivate Shift if no input for the timeout duration
  if (shiftActive && (currentTime - shiftLastTime > timeout)) {
    shiftActive = false;
  }

  // Deactivate AltGR if no input for the timeout duration
  if (altgrActive && (currentTime - altgrLastTime > timeout)) {
    altgrActive = false;
  }

  /*
  //Test for other .ino files
  if (enterKeyPressed) {
    Serial.println("ENTER_DETECTED");
    enterKeyPressed = false; // Reset the flag
  }

  if (deleteKeyPressed) {
     Serial.println("DELETE_DETECTED");
    deleteKeyPressed = false; // Reset the flag
  }

  if (keyPressDetected) {
    Serial.println("PRESS_DETECTED:");
    Serial.println(lastKeyPressed);
    keyPressDetected = false;
  }
  */

 
 if (escKeyPressed) {
    Serial.println("ESC_DETECTED");
    escKeyPressed = false; // Reset the flag
  }
 
 if (tabKeyPressed) {
    Serial.println("TAB_DETECTED");
    tabKeyPressed = false; // Reset the flag
  }

  if (numLockKeyPressed) {
    Serial.println("NUMLOCK_DETECTED");
    numLockKeyPressed = false; // Reset the flag
  }
  
  if (arrowUpPressed) {
    Serial.println("ARROW_UP_DETECTED");
    arrowUpPressed = false; // Reset the flag
  }
  
  if (arrowDownPressed) {
    Serial.println("ARROW_DOWN_DETECTED");
    arrowDownPressed = false; // Reset the flag
  }

  if (arrowRightPressed) {
    Serial.println("ARROW_RIGHT_DETECTED");
    arrowRightPressed = false; // Reset the flag
  }

  if (arrowLeftPressed) {
    Serial.println("ARROW_LEFT_DETECTED");
    arrowLeftPressed = false; // Reset the flag
  }

  if (volumeUpKeyPressed) {
    Serial.println("VOLUME_UP_DETECTED");
    volumeUpKeyPressed = false; // Reset the flag
  }
  /*
  if (volumeDownKeyPressed) {
    Serial.println("VOLUME_DOWN_DETECTED");
    volumeDownKeyPressed = false; // Reset the flag
  }

  if (brightnessUpKeyPressed) {
    Serial.println("BRIGHTNESS_UP_DETECTED");
    brightnessUpKeyPressed = false; // Reset the flag
  }

  if (brightnessDownKeyPressed) {
    Serial.println("BRIGHTNESS_DOWN_DETECTED");
    brightnessDownKeyPressed = false; // Reset the flag
  }
  */
}