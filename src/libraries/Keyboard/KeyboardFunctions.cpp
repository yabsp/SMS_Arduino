#include "keymaps.h"
#include "KeyboardFlags.h"
#include "KeyboardFunctions.h"

#define MAX_MESSAGE_LENGTH 1024

extern bool shiftActive;
extern bool altgrActive;
extern bool capsActive;
extern char input;
extern int shiftLastTime;
extern int altgrLastTime;

void add_char_to_message(char ch) {
    int len = strlen(message);
    if (len < MAX_MESSAGE_LENGTH) { // Ensure we don't exceed buffer size
        message[len] = ch;
        message[len + 1] = '\0';
    }
}

void delete_last_char_from_message() {
    int len = strlen(message);
    if (len > 0) {
        message[len - 1] = '\0';
    }
}

void handle_enter_key() {
    Serial.println("Message sent:");
    Serial.println(message);

    message[0] = '\0';
}

/*
void handle_key_press(uint16_t scanval, uint8_t lastscan, uint8_t lastlastscan){
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
} 
*/