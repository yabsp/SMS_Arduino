#include "Menu.h"
#include "KeyboardFlags.h"
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
Adafruit_GFX_Button New_Chat_Button; // For creating new Contact not chat
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
int Chat_Cursor_X = 4;
int Chat_Cursor_Y = 230;


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
  uint8_t visibleChats = 4; // Number of chats visible at a time
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
  if (Back_Button.justPressed() || escKeyPressed) {
    escKeyPressed = false;
    Back_Button.drawButton(false);
    Change_Menu(0);
  }
}


void Refresh_Chat_Menu() {
    Back_Button.press(Cursor_Pressed && Back_Button.contains(Cursor_X, Cursor_Y));
    if (Back_Button.justPressed() || escKeyPressed) {
        escKeyPressed = false;
        Back_Button.drawButton(false);
        Change_Menu(0);
    }

    New_Chat_Button.press(Cursor_Pressed && New_Chat_Button.contains(Cursor_X, Cursor_Y));
    if (New_Chat_Button.justPressed()) {
        New_Chat_Button.drawButton(false);
        Change_Menu(4);
    }

    scrollUpButton.press(Cursor_Pressed && scrollUpButton.contains(Cursor_X, Cursor_Y));
    if (scrollUpButton.justPressed() || arrowUpPressed) {
        arrowUpPressed = false;
        scrollUpButton.drawButton(false);
        if (chatOffset > 0) {
          chatOffset--; // Scroll up
          Draw_Chat_Menu();
        }
    }

    scrollDownButton.press(Cursor_Pressed && scrollDownButton.contains(Cursor_X, Cursor_Y));
    if (scrollDownButton.justPressed() || arrowDownPressed) {
        arrowDownPressed = false;
        scrollDownButton.drawButton(false);
        if (chatOffset + 4 < chatCount) {
            chatOffset++; // Scroll down
            Draw_Chat_Menu(); // Redraw menu
        }
    }

    // Check for button presses on visible chat buttons
    uint8_t visibleChats = 4; // Number of chats visible at a time
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
    activeInput = 0;
    Back_Button.drawButton(false);
    Change_Menu(2);
  }
  activeInput = 0;

  if (keyPressDetected){
    tft.setTextSize(1);
    tft.setTextColor(BLACK);
    if (lastKeyPressed.length() == 1) {
      Serial.println("in if statement");
      tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
      tft.print(lastKeyPressed);
      lastKeyPressed = "";
      Chat_Cursor_X += 6;
    } else if (lastKeyPressed.length() == 2){
      Serial.println("Doubleinput");
      tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
      tft.print(lastKeyPressed);
      lastKeyPressed = "";
      Chat_Cursor_X += 12;
    }
  } 

  if (deleteKeyPressed) {
    Serial.println("in deleteKeyPressed statement");
    deleteKeyPressed = false;
    if (Chat_Cursor_X > 4) {
      Chat_Cursor_X -= 6;
      tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y, 6, 12, WHITE);
    }
  } 
  if (enterKeyPressed_Screen) {
    enterKeyPressed_Screen = false;
    //enterKeyPressed_SMS = true;
    Chat_Cursor_X = 4;
    tft.fillRect(0, SCREEN_HEIGHT - 12 , SCREEN_WIDTH, 12, WHITE);
  }
  
  if (escKeyPressed) {
    activeInput = 0;
    Back_Button.drawButton(false);
    Change_Menu(2);
  }
  
}

void Refresh_Phone_Number_Selector() {

    Back_Button.press(Cursor_Pressed && Back_Button.contains(Cursor_X, Cursor_Y));
    if (Back_Button.justPressed() || escKeyPressed) {
        Back_Button.drawButton(false);
        escKeyPressed = false;
        Chat_Cursor_X = 4;
        Chat_Cursor_Y = 230;
        activeInput = 0;
        Change_Menu(2); // Return to Chat Menu
    }

    // Check if Phone Number field is tapped
    if (Cursor_Pressed && Cursor_X > 4 && Cursor_X < 316 && Cursor_Y > 80 && Cursor_Y < 125) {
        Serial.println("Bitte geben Sie die Telefonnummer ein:");

        tft.fillRect(5, 81, 310, 43, WHITE);  // Clear the field

        keyboardActive = true;

        tft.setCursor(10, 85);
        tft.print("Enter Phone Number:");

        Chat_Cursor_X = 10;
        Chat_Cursor_Y = 105;
        while(keyboardActive) {
          if (keyPressDetected){
            tft.setTextSize(1);
            tft.setTextColor(BLACK);
            if (lastKeyPressed.length() == 1) {
              Serial.println("in if statement");
              tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
              tft.print(lastKeyPressed);
              lastKeyPressed = "";
              Chat_Cursor_X += 6;
            } else if (lastKeyPressed.length() == 2){
              Serial.println("Doubleinput");
              //tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
              tft.print(lastKeyPressed);
              lastKeyPressed = "";
              Chat_Cursor_X += 12;
            }
          } 

          if (deleteKeyPressed) {
            Serial.println("in deleteKeyPressed statement");
            deleteKeyPressed = false;
            if (Chat_Cursor_X > 4) {
              Chat_Cursor_X -= 6;
              tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y, 6, 12, WHITE);
            }
          } 
          if (enterKeyPressed_Screen) {
            phoneNumber = message; // Use Serial Monitor to input the phone number
            message = "";
            activeInput = 0;
            enterKeyPressed_Screen = false;
            keyboardActive = false;
            tft.fillRect(0, SCREEN_HEIGHT - 12 , SCREEN_WIDTH, 12, WHITE);
            tft.setTextSize(1);
            Chat_Cursor_X = 4;
            Chat_Cursor_Y = 230;
          }
          
          if (escKeyPressed) {
            activeInput = 0;
            Chat_Cursor_X = 4;
            Chat_Cursor_Y = 230;
            message = "";
            Back_Button.drawButton(false);
            tft.setTextSize(2);
            Change_Menu(2);
          }

        }

        if (!escKeyPressed) {
          tft.fillRect(5, 81, 310, 43, WHITE);  // Clear the field
          //tft.setCursor(10, 95);
          //tft.setTextSize(1);
          tft.setCursor(18, 95);
          tft.setTextSize(2);
          tft.print(phoneNumber); // Display entered phone number on the screen
          Serial.print("Eingegebene Telefonnummer: ");
          Serial.println(phoneNumber); // Debugging output
          tft.setTextSize(1);
        }
    }

    // Check if Contact Name field is tapped
    if (Cursor_Pressed && Cursor_X > 4 && Cursor_X < 316 && Cursor_Y > 160 && Cursor_Y < 205) {
        Serial.println("Bitte geben Sie den Kontaktnamen ein:");

        tft.fillRect(5, 161, 310, 43, WHITE); // Clear the field

        keyboardActive = true; // Enter to confirm -> deactivate keyboard again

        tft.setCursor(10, 165);
        tft.print("Enter Contact Name:");

        Chat_Cursor_X = 10;
        Chat_Cursor_Y = 185;
        while(keyboardActive) {
          if (keyPressDetected){
            tft.setTextSize(1);
            tft.setTextColor(BLACK);
            if (lastKeyPressed.length() == 1) {
              Serial.println("in if statement");
              tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
              tft.print(lastKeyPressed);
              lastKeyPressed = "";
              Chat_Cursor_X += 6;
            } else if (lastKeyPressed.length() == 2){
              Serial.println("Doubleinput");
              tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
              tft.print(lastKeyPressed);
              lastKeyPressed = "";
              Chat_Cursor_X += 12;
            }
          } 

          if (deleteKeyPressed) {
            Serial.println("in deleteKeyPressed statement");
            deleteKeyPressed = false;
            if (Chat_Cursor_X > 4) {
              Chat_Cursor_X -= 6;
              tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y, 6, 12, WHITE);
            }
          } 
          if (enterKeyPressed_Screen) {
            contactName = message; // Use Serial Monitor to input the contact name
            message = "";
            enterKeyPressed_Screen = false;
            keyboardActive = false;
            tft.fillRect(0, SCREEN_HEIGHT - 12 , SCREEN_WIDTH, 12, WHITE);
            tft.setTextSize(1);
            Chat_Cursor_X = 4;
            Chat_Cursor_Y = 230;
          }
          
          if (escKeyPressed) {
            activeInput = 0;
            Chat_Cursor_X = 4;
            Chat_Cursor_Y = 230;
            message = "";
            tft.setTextSize(2);
            Back_Button.drawButton(false);
            Change_Menu(2);
          }
        }

        if(!escKeyPressed) {
          tft.fillRect(5, 161, 310, 43, WHITE); // Clear the field
          tft.setCursor(18, 175);
          tft.setTextSize(2);

          //tft.setCursor(10, 175);
          //tft.setTextSize(1);
          tft.print(contactName); // Display entered contact name on the screen
          Serial.print("Eingegebener Kontaktname: ");
          Serial.println(contactName); // Debugging output
          tft.setTextSize(1);
        }
    }

    // Check if Enter Button is pressed
    Enter_Button.press(Cursor_Pressed && Enter_Button.contains(Cursor_X, Cursor_Y));
    if (Enter_Button.justPressed() || enterKeyPressed_Screen) {
        enterKeyPressed_Screen = false;
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
          phoneNumber = "";
          contactName = "";
          message = "";
          delay(100);
          tft.fillScreen(WHITE);
          tft.fillRect(0, 0, 340, 30, TURQUOISE);
          tft.setTextColor(BLACK);
          tft.setTextSize(2);
          tft.setCursor(4, 7);
          tft.print("Input Error");
          tft.setCursor(20, 95);
          tft.print("Missing Phone Number or");
          tft.setCursor(40, 140);
          tft.print("Missing Contact Name");
          tft.setTextSize(1);
          tft.setCursor(4, 225);
          tft.print("Press Enter Key to continue!");
          Serial.println("Fehler: Telefonnummer oder Kontaktnamen fehlen!");
          while (!enterKeyPressed_Screen) {

          }
          enterKeyPressed_Screen = false;
      }
      Change_Menu(2); // Return to Chat Menu
    }
}



// Change Menus --------------------------------------------------------------------------------------------
void Change_Menu(uint8_t no){
    Current_Menu = no;
    switch(no){ // Change to the drawn menu
      case 0:
        keyboardActive = false;
        Draw_Main_Menu();
        break;
      case 1:
        keyboardActive = false;
        Draw_Settings_Menu();
        break;
      case 2:
        keyboardActive = false;
        Draw_Chat_Menu();
        break;
      case 3:
        keyboardActive = true;
        Draw_Chat_Viewer(phoneNumber, contactName);
        break;
      case 4:
        keyboardActive = false;
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
  New_Chat_Button.initButton(&tft, calculateRight(100) + 316, calculateTop(20) + 32, 100, 20, BLACK, BLACK, WHITE, "New Contact", 1);
  Enter_Button.initButton(&tft, calculateRight(50) + 316 , calculateTop(20) + 32, 50, 20, BLACK, BLACK, WHITE, "Enter", 1);
}
// Function to show field selector ------------------------------------------------------------------------------------
void Blink_Underscore(uint16_t x, uint16_t y) {
  static unsigned long lastBlink = 0;
  static bool underscoreVisible = false;

  if (activeInput > 0) {
    if (millis() - lastBlink > 500) { // Toggle every 500ms
      lastBlink = millis();
      underscoreVisible = !underscoreVisible;

      tft.fillRect(x, y, 316, 8, WHITE);
          
      }

      // Calculate dynamic position of the underscore
      //int underscoreX = baseX + inputLength * 6; // Each character in size 1 takes 6 pixels

    if (underscoreVisible) {
      tft.setTextColor(BLACK);
      tft.setCursor(x, y); // Position after the current input
      tft.print("_");
    }
    
  }
}
