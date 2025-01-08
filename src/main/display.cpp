#include "display.h"

int Cursor_X = 0;
int Cursor_Y = 0;
bool Cursor_Pressed = false;
uint8_t Current_Menu = 0;
int testing = 0;

MCUFRIEND_kbv tft; // Create display object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setupDisplay(){
// Initialize serial communication for debugging (optional)
  InitializeTouchscreen(tft);
  InitializeButtons(tft);
  Serial.println("Initialized all");
  Change_Menu(0); // Start in Main_Menu
  
  //newContact("+41786936406", "Yanick");
  //newContact("+41796299706", "Mike");
  newContact("+491605895978", "Max");
  newContact("+41794410255", "Gioia");
  /*
  Serial.println("Initializing SD card...");

  if (!SD.begin(chipSelect)){
    Serial.println("SD card initialization failed!");
        // Optional: Add error-handling code here
        return;
    }
    Serial.println("SD card initialized successfully.");
    */
}

void loopDisplay(){
  Cursor_Pressed = Touch_getXY(Cursor_X, Cursor_Y);
  Refresh_Menu(); // refresh on press
  Blink_Underscore();
  while (testing < 1){
    saveMessageInMemory("+41786936406","TestSpichty","2025");
    saveMessageInMemory("+41794410255", "TestGioia", "2025");
    saveMessageInMemory("+491605895978", "TestMax", "2025");
    saveMessageInMemory("+41794410255", "TestGioia2", "2025");
    saveMessageInMemory("+491605895978", "TestMax2", "2025");
    saveMessageInMemory("+41794410255", "TestGioia3", "2025");
    saveMessageInMemory("+41794410255", "TestGioia4", "2025");
    saveMessageInMemory("+41794410255", "TestGioia5", "2025");
    saveMessageInMemory("+41794410255", "TestGioia6", "2025");
    saveMessageInMemory("+41794410255", "TestGioia7", "2025");
    saveMessageInMemory("+41794410255", "TestGioia8", "2025");
    saveMessageInMemory("+41788802667", "TestGaby", "2025");
  }
}