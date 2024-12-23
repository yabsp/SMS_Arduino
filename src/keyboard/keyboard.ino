#define CLOCK 7 //D-
#define DATA 6  //D+

#include <LiquidCrystal.h>  //Best imported by library manager

#include "Keymaps.h"
#include "KeyboardFlags.h"
#include "KeyboardVariables.h"
#include "KeyboardFunctions.h"
/*
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
*/
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
  //pinMode(13, OUTPUT);
  /*
  lcd.begin(20, 4);
  lcd.cursor();
  lcd.blink();
  */
  bitSet(PCICR, PCIE2); // Enable pin change interrupts on pin D0-D7
  bitSet(PCMSK2, CLOCK); // Pin change interrupt on Clock pin
 }


ISR(PCINT2_vect)
{
  uint16_t scanval = 0;
  for(int i = 0; i<11; i++)
  {
    while(digitalRead(CLOCK));
    scanval |= digitalRead(DATA) << i; // 1 start, 8 data bits, 1 parity, 1 stop bit
    while(!digitalRead(CLOCK));
  }

  scanval >>= 1; // ignore the start bit
  scanval &= 0xFF; // ignore the parity and stop bit, isolate 8 data bits

  if(lastscan != 0xF0 && scanval != 0xF0){
    if (scanval == 0x12 || scanval == 0x59) { // Shift press
      shiftLastTime = millis(); // Reset Shift timer
      shiftActive = true;
    } else if (scanval == 0x11 && lastscan == 0xE0) { // AltGR press
      altgrActive = true;
      altgrLastTime = millis();
    } else if (scanval == 0x58){
      capsActive = !capsActive;
    }else {
      char output;
    }
	  switch(scanval)
	  {
		case 0x5A: //Enter
      enterKeyPressed = true;
      handle_enter_key();
      Serial.println("ENTER_FOR_NEW_LINE");
		  //lcd.setCursor(0, ++line & 0x03);
		  //col = 0;
		  break;
		case 0x66: //Backspace
      if (strlen(message) > 0) {
        deleteKeyPressed = true;
        delete_last_char_from_message();
        Serial.println("DELETE_LAST_CHAR_ON_LCD_SCREEN");
        /*
        lcd.setCursor(--col, line);
        lcd.write(' ');
        lcd.setCursor(col, line);
        */
      }
		break;
    
    /*
    case 0x4C: //Ö
      input = "oe";
      col++;
    case 0x52: //Ä
      input = "ae";
      col++;
    case 0x54: //Ü
      input = "ue";
      col++;
    */

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
          //lcd.write(input);
          Serial.println(input);

          //col++;
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
  unsigned long currentTime = millis();

  // Deactivate Shift if no input for the timeout duration
  if (shiftActive && (currentTime - shiftLastTime > timeout)) {
    shiftActive = false;
  }

  // Deactivate AltGR if no input for the timeout duration
  if (altgrActive && (currentTime - altgrLastTime > timeout)) {
    altgrActive = false;
  }

  digitalWrite(13, LOW);
  delay(500);  
  digitalWrite(13, HIGH);
  delay(500);

  /* Test for other .ino files
  if (enterKeyPressed) {
    Serial.println("ENTER_DETECTED");
    enterKeyPressed = false; // Reset the flag
  }
  if (deleteKeyPressed) {
     Serial.println("DELETE_DETECTED");
    deleteKeyPressed = false; // Reset the flag
  }
  */

}