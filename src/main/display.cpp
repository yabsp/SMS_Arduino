#include "display.h"
#include "SDReaderWriter.h"

int Cursor_X = 0;
int Cursor_Y = 0;
bool Cursor_Pressed = false;
uint8_t Current_Menu = 0;
int testing = 0;

extern volatile bool keyboardActive;
extern int Chat_Cursor_X;
extern int Chat_Cursor_Y;

MCUFRIEND_kbv tft; // Create display object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setupDisplay(){
  int contactCount = getContactsCount();
  for (int i = 0; i < contactCount; i++) {

    String result = getPhoneNumberAndContactNameByIndex(i);

    if (result == ""){
      Serial.println("Failed to retrieve contact.");
    } else{
      int separatorIndex = result.indexOf('_');
      String phoneNumber = result.substring(0, separatorIndex);
      String contactName = result.substring(separatorIndex + 1);

      Serial.println("Phone Number: " + phoneNumber);
      Serial.println("Contact Name: " + contactName);
      newContact(phoneNumber, contactName);
    }
  }
    InitializeTouchscreen(tft);
    InitializeButtons(tft);
    Serial.println("Initialized all");
    Change_Menu(0);
}

void loopDisplay(){
  Cursor_Pressed = Touch_getXY(Cursor_X, Cursor_Y);
  Refresh_Menu();
}
