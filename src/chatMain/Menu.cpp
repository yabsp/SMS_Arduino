#include "Menu.h"
// ChatHandler.h
#ifndef CHATHANDLER_H
#define CHATHANDLER_H

#include <Arduino.h>
String getChatMessages(const String &phoneNumber);

#endif

// Clickable Buttons ----------------------------------------------------------------------------------------
Adafruit_GFX_Button Main_Menu;
Adafruit_GFX_Button Settings_Menu;
Adafruit_GFX_Button Back_Button;
Adafruit_GFX_Button Chat_Menu;
Adafruit_GFX_Button New_Chat_Button;
Adafruit_GFX_Button Enter_Button;
Adafruit_GFX_Button chatButtons[MAX_CHATS];
Adafruit_GFX_Button scrollDownButton;
Adafruit_GFX_Button scrollUpButton;


// Variables --------------------------------------------------------------------------------------
int activeInput = 0; // (0 = none, 1 = phoneNumber, 2 = contactName, 3 = message)
int inputLength = 0; // current length of the input
String phoneNumber;
String contactName;
uint8_t chatOffset = 0; // Tracks the scroll position for visible chats


// Draw Menu Functions --------------------------------------------------------------------------------------
void Draw_Main_Menu() {
  // Background and Text
  tft.fillScreen(TURQUOISE);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(50,90);
  tft.print("University of Basel");
  tft.setCursor(50,120);
  tft.print("Arduino SMS Sender");
  tft.setCursor(50,150);
  tft.setTextSize(1);
  tft.print("A Project by Max, Mike, Gioia and Yanick");
  //Buttons
  Settings_Menu.drawButton(true);
  Chat_Menu.drawButton(true);
}


void Draw_Settings_Menu() {
  // Menubar
  tft.fillScreen(WHITE);
  tft.fillRect(0, 0, tft.width(), 28, TURQUOISE);
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  tft.setCursor(6, 6);
  tft.setTextSize(2);
  tft.print("Settings");
  /*tft.setCursor(215, 4);
  tft.setTextSize(1);
  tft.print("22.12.2024, 11:46");
  tft.setCursor(215, 16);
  tft.print("Basel, 29 ° C");*/
  tft.drawLine(0, 56, 320, 56, BLACK);
  Back_Button.drawButton(true);
}


void Draw_Chat_Menu() {
  // Menubar
  tft.fillScreen(WHITE);
  tft.fillRect(0, 0, tft.width(), 28, TURQUOISE);
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  tft.setCursor(6, 6);
  tft.setTextSize(2);
  tft.print("Chat");
  tft.drawLine(0, 56, 320, 56, BLACK);
  Back_Button.drawButton(true);
  New_Chat_Button.drawButton(true);

  scrollUpButton.initButton(&tft, 290, 78, 50, 30, BLACK, BLACK, LIGHTGREY, "Up", 1);
  scrollDownButton.initButton(&tft, 290, calculateBottom(30) + 236 , 50, 30, BLACK, BLACK, LIGHTGREY, "Down", 1);

  // Create buttons for each visible chat (based on chatOffset)
  uint8_t visibleChats = 5; // Number of chats visible at a time
  for (uint8_t i = 0; i < visibleChats && (i + chatOffset) < chatCount; i++) {
    String label = chatList[i + chatOffset].contactName;
    if (chatList[i + chatOffset].unreadMessages > 0) {
      label += " (" + String(chatList[i + chatOffset].unreadMessages) + ")";
    }
        
    chatButtons[i].initButton(
      &tft,
      calculateLeft(250) + 4,     // X-Coordinate
      78 + i * 40,                // Y-Coordinate
      250, 30,                    // Width and height
      BLACK, BLACK, LIGHTGREY,    // Border, text, and fill colors
      chatList[i + chatOffset].contactName.c_str(), // Contact name as label
      1 // Text size
      );
    chatButtons[i].drawButton(true);
  }
  scrollUpButton.drawButton(true);
  scrollDownButton.drawButton(true);
}


void Draw_Chat_Viewer(String phoneNumber, String contactName) {
  // Menubar
  tft.fillScreen(WHITE);
  tft.fillRect(0, 0, tft.width(), 28, TURQUOISE);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setCursor(6, 6);
  tft.print(contactName);
  tft.drawLine(0, 56, 320, 56, BLACK);
  Back_Button.drawButton(true);

  String messages = getChatMessages(phoneNumber);

// Display the messages line by line
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  int cursorX = 4;
  int cursorY = 60;

  for (uint16_t i = 0; i < messages.length(); i++) {
    if (messages[i] == '\n') {    // Move to the next line if newline character is found
      cursorY += 10;
      cursorX = 4;
    } else {
      tft.setCursor(cursorX, cursorY);
      tft.print(messages[i]);
      cursorX += 6;
    }
  }
}

void Draw_Phone_Number_Selector() {
  // Menubar
  tft.fillScreen(WHITE);
  tft.fillRect(0, 0, tft.width(), 28, TURQUOISE);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setCursor(6, 6);
  tft.print("New Contact");
  tft.drawLine(0, 56, 320, 56, BLACK);
  Back_Button.drawButton(true);
  Enter_Button.drawButton(true);

  // Draw Phone Number Field
  tft.setTextSize(2);
  tft.setCursor(4, 60); // Header for phone number field
  tft.setTextColor(BLACK);
  tft.print("Phone Number:");

  tft.drawRect(4, 80, 312, 45, BLACK);
  tft.setTextSize(1);
  tft.setCursor(81, 95); // Adjusted Y-coordinate for centered text
  tft.print("Click to enter phone number");

  // Draw Contact Name Field
  tft.setTextSize(2);
  tft.setCursor(4, 140); // Header for contact name field
  tft.print("Contact Name:");

  tft.drawRect(4, 160, 312, 45, BLACK);
  tft.setTextSize(1);
  tft.setCursor(81, 175); // Adjusted Y-coordinate for centered text
  tft.print("Click to enter contact name");

  //createChatFromSerialInput();
}


// Handle Menu Functions --------------------------------------------------------------------------------------
void Refresh_Main_Menu() {
  // Settings_Menu
  Settings_Menu.press(Cursor_Pressed && Settings_Menu.contains(Cursor_X, Cursor_Y));
  if (Settings_Menu.justPressed()){
    Settings_Menu.drawButton(false);
    Change_Menu(1);
  }

  Chat_Menu.press(Cursor_Pressed && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    Chat_Menu.drawButton(false);
    Change_Menu(2);
  }
}


void Refresh_Settings_Menu() {
  Back_Button.press(Cursor_Pressed && Back_Button.contains(Cursor_X, Cursor_Y));
  if (Back_Button.justPressed()) {
    Back_Button.drawButton(false);
    Change_Menu(0);
  }
}


void Refresh_Chat_Menu() {
    Back_Button.press(Cursor_Pressed && Back_Button.contains(Cursor_X, Cursor_Y));
    if (Back_Button.justPressed()) {
        Back_Button.drawButton(false);
        Change_Menu(0);
    }

    New_Chat_Button.press(Cursor_Pressed && New_Chat_Button.contains(Cursor_X, Cursor_Y));
    if (New_Chat_Button.justPressed()) {
        New_Chat_Button.drawButton(false);
        Change_Menu(4);
    }

    scrollUpButton.press(Cursor_Pressed && scrollUpButton.contains(Cursor_X, Cursor_Y));
    if (scrollUpButton.justPressed()) {
        scrollUpButton.drawButton(false);
        if (chatOffset > 0) {
          chatOffset--; // Scroll up
          Draw_Chat_Menu();
        }
    }

    scrollDownButton.press(Cursor_Pressed && scrollDownButton.contains(Cursor_X, Cursor_Y));
    if (scrollDownButton.justPressed()) {
        scrollDownButton.drawButton(false);
        if (chatOffset + 5 < chatCount) {
            chatOffset++; // Scroll down
            Draw_Chat_Menu(); // Redraw menu
        }
    }

    // Check for button presses on visible chat buttons
    uint8_t visibleChats = 5; // Number of chats visible at a time
    for (uint8_t i = 0; i < visibleChats && (i + chatOffset) < chatCount; i++) {
        chatButtons[i].press(Cursor_Pressed && chatButtons[i].contains(Cursor_X, Cursor_Y));
        if (chatButtons[i].justPressed()) {
            chatButtons[i].drawButton(false);
            phoneNumber = chatList[i + chatOffset].phoneNumber;
            contactName = chatList[i + chatOffset].contactName;
            Change_Menu(3); // Go to chat viewer
        }
    }
}


void Refresh_Chat_Viewer() {
  Back_Button.press(Cursor_Pressed && Back_Button.contains(Cursor_X, Cursor_Y));
  activeInput = 3; // messageInput
  if (Back_Button.justPressed()){
    Back_Button.drawButton(false);
    activeInput = 0;
    Change_Menu(2);
  }
}

void Refresh_Phone_Number_Selector() {
    Back_Button.press(Cursor_Pressed && Back_Button.contains(Cursor_X, Cursor_Y));
    if (Back_Button.justPressed()) {
        Back_Button.drawButton(false);
        activeInput = 0;
        Change_Menu(2); // Return to Chat Menu
    }

    // Check if Phone Number field is tapped
    if (Cursor_Pressed && Cursor_X > 4 && Cursor_X < 316 && Cursor_Y > 80 && Cursor_Y < 125) {
        Serial.println("Bitte geben Sie die Telefonnummer ein:");
        phoneNumber = buildStringFromInput(); // Use Serial Monitor to input the phone number
        tft.fillRect(5, 81, 310, 43, WHITE);  // Clear the field
        tft.setCursor(10, 95);
        tft.setTextSize(1);
        tft.print(phoneNumber); // Display entered phone number on the screen
        Serial.print("Eingegebene Telefonnummer: ");
        Serial.println(phoneNumber); // Debugging output
    }

    // Check if Contact Name field is tapped
    if (Cursor_Pressed && Cursor_X > 4 && Cursor_X < 316 && Cursor_Y > 160 && Cursor_Y < 205) {
        Serial.println("Bitte geben Sie den Kontaktnamen ein:");
        contactName = buildStringFromInput(); // Use Serial Monitor to input the contact name
        tft.fillRect(5, 161, 310, 43, WHITE); // Clear the field
        tft.setCursor(10, 175);
        tft.setTextSize(1);
        tft.print(contactName); // Display entered contact name on the screen
        Serial.print("Eingegebener Kontaktname: ");
        Serial.println(contactName); // Debugging output
    }

    // Check if Enter Button is pressed
    Enter_Button.press(Cursor_Pressed && Enter_Button.contains(Cursor_X, Cursor_Y));
    if (Enter_Button.justPressed()) {
        Enter_Button.drawButton(false);

        // Ensure both fields are filled
    if (phoneNumber.length() > 0 && contactName.length() > 0) {
            newContact(phoneNumber, contactName); // Create new contact
            Serial.println("Neuer Kontakt erfolgreich erstellt:");
            Serial.print("Telefonnummer: ");
            Serial.println(phoneNumber);
            Serial.print("Kontaktname: ");
            Serial.println(contactName);
        } else {
            Serial.println("Fehler: Telefonnummer oder Kontaktnamen fehlen!");
        }
        Change_Menu(2); // Return to Chat Menu
    }
}



// Change Menus --------------------------------------------------------------------------------------------
void Change_Menu(uint8_t no){
    Current_Menu = no;
    switch(no){ // Change to the drawn menu
      case 0:
        Draw_Main_Menu();
        break;
      case 1:
        Draw_Settings_Menu();
        break;
      case 2:
        Draw_Chat_Menu();
        break;
      case 3:
        Draw_Chat_Viewer(phoneNumber, contactName);
        break;
      case 4:
        Draw_Phone_Number_Selector();
        break;
    }
}


// Refresh Menus --------------------------------------------------------------------------------------------
void Refresh_Menu(){
  switch(Current_Menu){ // Refesh the current menu
    case 0:
      Refresh_Main_Menu();
      break;
    case 1:
      Refresh_Settings_Menu();
      break;
    case 2:
      Refresh_Chat_Menu();
      break;
    case 3:
      Refresh_Chat_Viewer();
      break;
    case 4:
      Refresh_Phone_Number_Selector();
      break;
  }
}


// Function to initialize all buttons ------------------------------------------------------------------------------------
void InitializeButtons(MCUFRIEND_kbv &tft) {
  Back_Button.initButton(
    &tft,                   // Displaypointer
    calculateLeft(50) + 4,  // X-Coordinate
    calculateTop(20) + 32,  // Y-Coordinate
    50, 20,                 // Width and height
    BLACK, BLACK, WHITE,    // Border, text, and fill colors
    "Back",                 // Label
    1                       // Text size
  );
  Settings_Menu.initButton(&tft, calculateLeft(145) + 10, calculateTop(24) + 175, 145, 24, BLACK, BLACK, WHITE, "Settings", 1);
  Chat_Menu.initButton(&tft, calculateRight(145) + 310, calculateTop(24) + 175, 145, 24, BLACK, BLACK, WHITE, "Chats", 1);
  New_Chat_Button.initButton(&tft, calculateRight(100) + 316, calculateTop(20) + 32, 100, 20, BLACK, BLACK, WHITE, "New Contact", 1); // is now new contact
  Enter_Button.initButton(&tft, calculateRight(50) + 316 , calculateTop(20) + 32, 50, 20, BLACK, BLACK, WHITE, "Enter", 1);
}

// Function to show field selector ------------------------------------------------------------------------------------
void Blink_Underscore() {
  static unsigned long lastBlink = 0;
  static bool underscoreVisible = false;

  if (activeInput > 0) {
    if (millis() - lastBlink > 500) { // Toggle every 500ms
      lastBlink = millis();
      underscoreVisible = !underscoreVisible;

      // Calculate base positions for each field
      int baseX, baseY;
      switch (activeInput) {
        case 1: // Phone number input
          baseX = 8; // X-coordinate
          baseY = 100; // Y-coordinate
          tft.fillRect(baseX, baseY, 316, 8, WHITE);
          break;
        case 2: // Contact name input
          baseX = 8; // X-coordinate
          baseY = 180; // Y-coordinate
          tft.fillRect(baseX, baseY, 316, 8, WHITE);
          break;
        case 3: // Chat message input
          baseX = 4; // X-coordinate 
          baseY = 230; // Y-coordinate
          tft.fillRect(baseX - 4, baseY - 4, 340, 16, LIGHTGREY);
          break;
      }

      // Calculate dynamic position of the underscore
      int underscoreX = baseX + inputLength * 6; // Each character in size 1 takes 6 pixels

      if (underscoreVisible) {
        tft.setTextColor(BLACK);
        tft.setCursor(underscoreX, baseY); // Position after the current input
        tft.print("_");
      }
    }
  }
}
