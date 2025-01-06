// Libraries ------------------------------------------------------------------------------
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Config.h"
#include "Menu.h"
#include "TouchHandler.h"


// Define the shared global variables -----------------------------------------------------
int Cursor_X = 0;
int Cursor_Y = 0;
bool Cursor_Pressed = false;
uint8_t Current_Menu = 0;


// Create objects --------------------------------------------------------------
MCUFRIEND_kbv tft; // Create display object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // Create touchscreen object


// Setup -----------------------------------------------------------------------
void setup() {
  // Initialize serial communication for debugging (optional)
  Serial.begin(9600);
  InitializeTouchscreen(tft);
  InitializeButtons(tft);
  Serial.println("Initialized all");
  Change_Menu(0); // Start in Main_Menu
  
  newContact("+1678543590", "Yanick");
  newContact("+9876543210", "Osman");
  newContact("+3781293733", "Tschudin");
  newContact("+1234567320", "Mike");
  newContact("+2813792183", "Max");
  newContact("+4857983455", "Gioia");
  /*addMessageToChat("+1678543590", "Hallo");
  sendMessage("+1678543590", "Hello Back");*/
}


// Main loop -------------------------------------------------------------------
void loop() {
  Cursor_Pressed = Touch_getXY(Cursor_X, Cursor_Y);
  Refresh_Menu(); // refresh on press
  Blink_Underscore();
}