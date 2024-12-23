#define CLOCK 7 //D-
#define DATA 6  //D+

#include "Keymaps.h"
#include "KeyboardFlags.h"
#include "KeyboardVariables.h"
#include "KeyboardFunctions.h"

volatile bool enterKeyPressed = false;
volatile bool deleteKeyPressed = false;
volatile bool keyPressDetected = false;
char lastKeyPressed = '\0'; 
char message[MAX_MESSAGE_LENGTH + 1] = ""; // Message buffer

void setup()
{
  Serial.begin(115200);
  pinMode(CLOCK, INPUT_PULLUP); //For most keyboards the builtin pullups are sufficient, so the 10k pullups can be omitted
  pinMode(DATA, INPUT_PULLUP);
  bitSet(PCICR, PCIE2); // Enable pin change interrupts on pin D0-D7
  bitSet(PCMSK2, CLOCK); // Pin change interrupt on Clock pin
 }


ISR(PCINT2_vect)
{
  
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

	  switch(scanval)
	  {
		case 0x5A: //Enter
      enterKeyPressed = true;
      handle_enter_key();
      Serial.println("ENTER_FOR_NEW_LINE");
		  break;
		case 0x66: //Backspace
      if (strlen(message) > 0) {
        deleteKeyPressed = true;
        delete_last_char_from_message();
        Serial.println("DELETE_LAST_CHAR_ON_LCD_SCREEN");
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
  
  bitSet(PCIFR, PCIF2);
}

void loop()
{
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

}