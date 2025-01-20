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
// Initialize serial communication for debugging (optional)
  
  //newContact("+41786936406", "Yanick");
  //newContact("+41796299706", "Mike");
  newContact("+491605895978", "Max");
  //newContact("+41794410255", "Gioia");
  newContact("+411605895978", "Dummy1");
  newContact("+421605895978", "Fuck");
  newContact("+431605895978", "Foodin");
  /*
  Serial.println("Initializing SD card...");

  if (!SD.begin(chipSelect)){
    Serial.println("SD card initialization failed!");
        // Optional: Add error-handling code here
        return;
    }
    Serial.println("SD card initialized successfully.");
    */

    storeContact("+491605895978", "Max");
    storeMessage("+491605895978", "2025-01-03_14-51-43_0", "Test 1");
    storeMessage("+491605895978", "2025-01-03_14-51-48_1", "Test 2");
    storeMessage("+491605895978", "2025-01-03_14-51-50_0", "Test 3");
    storeMessage("+491605895978", "2025-01-03_14-51-53_0", "Test 4");
    storeMessage("+491605895978", "2025-01-03_14-51-57_0", "Test 5");
    storeMessage("+491605895978", "2025-01-03_14-51-59_1", "Test 6");
    storeMessage("+491605895978", "2025-01-03_14-52-23_1", "Test 7");
    storeMessage("+491605895978", "2025-01-03_14-52-36_0", "Test 8");
    storeMessage("+491605895978", "2025-01-03_14-52-45_1", "Test 9");
    storeMessage("+491605895978", "2025-01-03_14-52-56_0", "Test 10");

    storeContact("+411605895978", "Dummy1");
    storeMessage("+411605895978", "2025-01-03_14-51-43_0", "Test 1");
    storeMessage("+411605895978", "2025-01-03_14-51-48_1", "Test 2");
    storeMessage("+411605895978", "2025-01-03_14-51-50_0", "Test 3");
    storeMessage("+411605895978", "2025-01-03_14-51-53_0", "Test 4");

    storeContact("+421605895978", "Fuck");
    storeMessage("+421605895978", "2025-01-03_14-52-45_1", "Test 9");
    storeMessage("+421605895978", "2025-01-03_14-52-56_0", "Test 10");


    storeContact("+431605895978", "Foodin");
    storeMessage("+431605895978", "2025-01-03_14-51-57_0", "Test 5");
    storeMessage("+431605895978", "2025-01-03_14-51-59_1", "Test 6");
    storeMessage("+431605895978", "2025-01-03_14-52-23_1", "Test 7");

    InitializeTouchscreen(tft);
    InitializeButtons(tft);
    Serial.println("Initialized all");
    Change_Menu(0); // Start in Main_Menu
}

void loopDisplay(){
  Cursor_Pressed = Touch_getXY(Cursor_X, Cursor_Y);
  Refresh_Menu(); // refresh on press

  //while (testing < 1){

    // Contact 1
    //saveMessageInMemory("+41786936406","TestSpichty","2025");
    
    // Contact 2
    //saveMessageInMemory("+491605895978", "TestMax", "2025-01-03_14-51-43_0");
    //saveMessageInMemory("+491605895978", "TestMax2", "2025-01-03_14-56-21_1");

    /*
    // Contact 3
    saveMessageInMemory("+41794410255", "TestGioia2", "2025");
    saveMessageInMemory("+41794410255", "TestGioia", "2025");
    saveMessageInMemory("+41794410255", "TestGioia3", "2025");
    saveMessageInMemory("+41794410255", "TestGioia4", "2025");
    saveMessageInMemory("+41794410255", "TestGioia5", "2025");
    saveMessageInMemory("+41794410255", "TestGioia6", "2025");
    saveMessageInMemory("+41794410255", "TestGioia7", "2025");
    saveMessageInMemory("+41794410255", "TestGioia8", "2025");

    // Contact 4
    saveMessageInMemory("+41788802667", "TestGaby", "2025");

    // Contact 5
    saveMessageInMemory("+41786666689", "TestGaby", "2025");

    // Contact 6
    saveMessageInMemory("+41786666684", "TestGaby", "2025");

    // Contact 7
    saveMessageInMemory("+41756894322123", "Fudin", "2024-2025 Skiferien");
    */

  //}
}