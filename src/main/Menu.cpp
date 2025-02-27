#include "Menu.h"
#include "KeyboardFlags.h"
#include "SDReaderWriter.h"
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
Adafruit_GFX_Button scroll_Up_Chat_Button;
Adafruit_GFX_Button scroll_Down_Chat_Button;
Adafruit_GFX_Button sound_Turned_On_Button;
Adafruit_GFX_Button increase_Sound_Length_Button;
Adafruit_GFX_Button decrease_Sound_Length_Button;


// Variables --------------------------------------------------------------------------------------
int inputLength = 0; // current length of the input
String phoneNumber;
String contactName;
uint8_t chatOffset = 0; // Tracks the scroll position for visible chats
uint8_t visibleChats = 4;
extern int Chat_Cursor_X = 4;
extern int Chat_Cursor_Y = 230;
int tabCount = 0; // Used to cycle buttons
int16_t logo_X = 40;  // X coordinate of the center of the logo
int16_t logo_Y = 160; // Y coordinate of the center of the logo
extern volatile bool keyboardActive;
int8_t selectedChat = 0;
int8_t startIndexChat = getStoredMessagesCount(phoneNumber.c_str()) - 4; // starting index for messages in chat
uint16_t message_Cursor_X = 4;
uint16_t message_Cursor_Y = 66; // lower part of the character on screen
extern volatile bool refresh_Chat_View = false;
extern volatile bool sound_Switch_Active = true;
extern volatile bool increaseSoundTime;
extern volatile bool decreaseSoundTime;
extern volatile bool refresh_Chat_Menu;


// Draw Menu Functions --------------------------------------------------------------------------------------
void Draw_Main_Menu() {
  
  // Background and Text
  tft.fillScreen(TURQUOISE);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(48,90);
  tft.print("University of Basel");
  tft.setCursor(97,120);
  tft.print("SMS Machine");
  tft.setCursor(45,150);
  tft.setTextSize(1);
  tft.print("A Project by Gioia, Max, Mike and Yanick");
  
  // Logo
  for (int i = 0; i < 15; i++) {
    // Vertical lines
    tft.drawPixel(logo_Y-13, logo_X-30 + i, BLACK);
    tft.drawPixel(logo_Y-12, logo_X-30 + i, BLACK);
    tft.drawPixel(logo_Y-11, logo_X-30 + i, BLACK);

    tft.drawPixel(logo_Y-13, logo_X-6 + i, BLACK);
    tft.drawPixel(logo_Y-12, logo_X-6 + i, BLACK);
    tft.drawPixel(logo_Y-11, logo_X-6 + i, BLACK);

    tft.drawPixel(logo_Y-13, logo_X+18 + i, BLACK);
    tft.drawPixel(logo_Y-12, logo_X+18 + i, BLACK);
    tft.drawPixel(logo_Y-11, logo_X+18 + i, BLACK);


    tft.drawPixel(logo_Y+13, logo_X-30 + i, BLACK);
    tft.drawPixel(logo_Y+12, logo_X-30 + i, BLACK);
    tft.drawPixel(logo_Y+11, logo_X-30 + i, BLACK);

    tft.drawPixel(logo_Y+13, logo_X-6 + i, BLACK);
    tft.drawPixel(logo_Y+12, logo_X-6 + i, BLACK);
    tft.drawPixel(logo_Y+11, logo_X-6 + i, BLACK);

    tft.drawPixel(logo_Y+13, logo_X+18 + i, BLACK);
    tft.drawPixel(logo_Y+12, logo_X+18 + i, BLACK);
    tft.drawPixel(logo_Y+11, logo_X+18 + i, BLACK);

    // Horizontal lines
    tft.drawPixel(logo_Y-30 + i, logo_X-11, BLACK);
    tft.drawPixel(logo_Y-30 + i, logo_X-12, BLACK);
    tft.drawPixel(logo_Y-30 + i, logo_X-13, BLACK);

    tft.drawPixel(logo_Y-6 + i, logo_X-11, BLACK);
    tft.drawPixel(logo_Y-6 + i, logo_X-12, BLACK);
    tft.drawPixel(logo_Y-6 + i, logo_X-13, BLACK);

    tft.drawPixel(logo_Y+18 + i, logo_X-11, BLACK);
    tft.drawPixel(logo_Y+18 + i, logo_X-12, BLACK);
    tft.drawPixel(logo_Y+18 + i, logo_X-13, BLACK);


    tft.drawPixel(logo_Y-30 + i, logo_X+11, BLACK);
    tft.drawPixel(logo_Y-30 + i, logo_X+12, BLACK);
    tft.drawPixel(logo_Y-30 + i, logo_X+13, BLACK);

    tft.drawPixel(logo_Y-6 + i, logo_X+11, BLACK);
    tft.drawPixel(logo_Y-6 + i, logo_X+12, BLACK);
    tft.drawPixel(logo_Y-6 + i, logo_X+13, BLACK);

    tft.drawPixel(logo_Y+18 + i, logo_X+11, BLACK);
    tft.drawPixel(logo_Y+18 + i, logo_X+12, BLACK);
    tft.drawPixel(logo_Y+18 + i, logo_X+13, BLACK);

  }
  // Middle Cross
  for (int i = -1; i <=1 ; i++){
    for(int j = -1; j <= 1; j++) {
      tft.drawPixel(logo_Y-6+i, logo_X-6+j , BLACK);
      tft.drawPixel(logo_Y-3+i, logo_X-3+j , BLACK);
      tft.drawPixel(logo_Y-6+i, logo_X+6+j , BLACK);
      tft.drawPixel(logo_Y-3+i, logo_X+3+j , BLACK);
      tft.drawPixel(logo_Y+6+i, logo_X-6+j , BLACK);
      tft.drawPixel(logo_Y+3+i, logo_X-3+j , BLACK);
      tft.drawPixel(logo_Y+6+i, logo_X+6+j , BLACK);
      tft.drawPixel(logo_Y+3+i, logo_X+3+j , BLACK);
    }
  }
  //Diagonal Lines
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      // Top Left
      tft.drawPixel(logo_Y-27+i, logo_X-27+j, BLACK);
      tft.drawPixel(logo_Y-24+i, logo_X-24+j, BLACK);
      tft.drawPixel(logo_Y-21+i, logo_X-21+j, BLACK);
      tft.drawPixel(logo_Y-18+i, logo_X-18+j, BLACK);

      // Top Right
      tft.drawPixel(logo_Y+27+i, logo_X-27+j, BLACK);
      tft.drawPixel(logo_Y+24+i, logo_X-24+j, BLACK);
      tft.drawPixel(logo_Y+21+i, logo_X-21+j, BLACK);
      tft.drawPixel(logo_Y+18+i, logo_X-18+j, BLACK);

      // Bottom Left
      tft.drawPixel(logo_Y-27+i, logo_X+27+j, BLACK);
      tft.drawPixel(logo_Y-24+i, logo_X+24+j, BLACK);
      tft.drawPixel(logo_Y-21+i, logo_X+21+j, BLACK);
      tft.drawPixel(logo_Y-18+i, logo_X+18+j, BLACK);

      // Bottom Right
      tft.drawPixel(logo_Y+27+i, logo_X+27+j, BLACK);
      tft.drawPixel(logo_Y+24+i, logo_X+24+j, BLACK);
      tft.drawPixel(logo_Y+21+i, logo_X+21+j, BLACK);
      tft.drawPixel(logo_Y+18+i, logo_X+18+j, BLACK);
    }
  }

  // Outter Crosses
  for (int i = -1; i <= 1; i++){
    for (int j = -1; j <= 1; j++){
      // Top
      tft.drawPixel(logo_Y-3+i, logo_X-27+j, BLACK);
      tft.drawPixel(logo_Y+3+i, logo_X-27+j, BLACK);
      tft.drawPixel(logo_Y-3+i, logo_X-21+j, BLACK);
      tft.drawPixel(logo_Y+3+i, logo_X-21+j, BLACK);
      tft.drawPixel(logo_Y-6+i, logo_X-18+j, BLACK);
      tft.drawPixel(logo_Y+6+i, logo_X-18+j, BLACK);

      // Right
      tft.drawPixel(logo_Y-27+i, logo_X-3+j, BLACK);
      tft.drawPixel(logo_Y-27+i, logo_X+3+j, BLACK);
      tft.drawPixel(logo_Y-21+i, logo_X-3+j, BLACK);
      tft.drawPixel(logo_Y-21+i, logo_X+3+j, BLACK);
      tft.drawPixel(logo_Y-18+i, logo_X-6+j, BLACK);
      tft.drawPixel(logo_Y-18+i, logo_X+6+j, BLACK);

      // Bottom
      tft.drawPixel(logo_Y-3+i, logo_X+27+j, BLACK);
      tft.drawPixel(logo_Y+3+i, logo_X+27+j, BLACK);
      tft.drawPixel(logo_Y-3+i, logo_X+21+j, BLACK);
      tft.drawPixel(logo_Y+3+i, logo_X+21+j, BLACK);
      tft.drawPixel(logo_Y-6+i, logo_X+18+j, BLACK);
      tft.drawPixel(logo_Y+6+i, logo_X+18+j, BLACK);

      // Left
      tft.drawPixel(logo_Y+27+i, logo_X-3+j, BLACK);
      tft.drawPixel(logo_Y+27+i, logo_X+3+j, BLACK);
      tft.drawPixel(logo_Y+21+i, logo_X-3+j, BLACK);
      tft.drawPixel(logo_Y+21+i, logo_X+3+j, BLACK);
      tft.drawPixel(logo_Y+18+i, logo_X-6+j, BLACK);
      tft.drawPixel(logo_Y+18+i, logo_X+6+j, BLACK);
    }
  }

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
  tft.drawLine(0, 56, 320, 56, BLACK);
  Back_Button.drawButton(true);
 
  if (sound_Switch_Active) {
    sound_Turned_On_Button.initButton(&tft,225, 98, 50, 30, BLACK, BLACK, LIGHTGREY, "On", 1);
    sound_Turned_On_Button.drawButton(true);
  } else {
    sound_Turned_On_Button.initButton(&tft,225, 98, 50, 30, BLACK, BLACK, LIGHTGREY, "Off", 1);
    sound_Turned_On_Button.drawButton(true);
  }

  tft.setTextSize(1);
  tft.setCursor(6, 95);
  tft.print("Sound notifications are turned: ");
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

  for (uint8_t i = 0; i < visibleChats && (i + chatOffset) < chatCount; i++) {
    String label = chatList[i + chatOffset].contactName;
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
  tft.drawLine(0, 225, 320, 225, BLACK);
  Back_Button.drawButton(true);
  Serial.println(contactName);
  Serial.println(phoneNumber);
  recipientPhoneNumber = phoneNumber;

  scroll_Up_Chat_Button.initButton(&tft, 235, 42, 50, 20, BLACK, BLACK, WHITE, "Up", 1);
  scroll_Down_Chat_Button.initButton(&tft, 290, 42, 50, 20, BLACK, BLACK, WHITE, "Down", 1);

  scroll_Up_Chat_Button.drawButton(true);
  scroll_Down_Chat_Button.drawButton(true);
  
  if (getStoredMessagesCount(phoneNumber.c_str()) <= 0) {

    Serial.println("No chat history");

    tft.fillRect(0, 56, 320, 169, WHITE);
    tft.drawLine(0, 56, 320, 56, BLACK);
    tft.drawLine(0, 225, 320, 225, BLACK);


    tft.fillRect(104, 126, 130, 16, LIGHTGREY);
    tft.drawRect(104, 126, 130, 16, BLACK);
    tft.drawRect(103, 125, 132, 17, BLACK);
    tft.drawRect(102, 124, 134, 18, BLACK);
    tft.setCursor(110, 130);
    tft.setTextColor(BLACK);
    tft.setTextSize(1);
    tft.println("No chat history yet!");
  } else if (getStoredMessagesCount(phoneNumber.c_str()) < 4) {
      tft.fillRect(0, 56, 320, 169, WHITE);
      tft.drawLine(0, 56, 320, 56, BLACK);
      tft.drawLine(0, 225, 320, 225, BLACK);
      loadMessages(phoneNumber.c_str(), 0, getStoredMessagesCount(phoneNumber.c_str()));
  } else {
      loadMessages(phoneNumber.c_str(), getStoredMessagesCount(phoneNumber.c_str()) - 4, 4);
      tft.drawLine(0, 56, 320, 56, BLACK);
      tft.drawLine(0, 225, 320, 225, BLACK);
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
}


// Handle Menu Functions --------------------------------------------------------------------------------------
void Refresh_Main_Menu() {

  if (tabCount == 0 && enterKeyPressed_Screen) {
    enterKeyPressed_Screen = false;
  }
  if (tabKeyPressed){

    tabKeyPressed = false;
    tabCount++;

    if(tabCount != 0 && tabCount % 2 == 1){ // In Settings Button

      Cursor_X = 83;
      Cursor_Y = 187;

      Settings_Menu.drawButton(false);
      Chat_Menu.drawButton(true);

    } else if (tabCount != 0 && tabCount % 2 == 0){ // In Chats Button
      
      Cursor_X = 207;
      Cursor_Y = 187;

      Settings_Menu.drawButton(true);
      Chat_Menu.drawButton(false);

    }
  }

  // Settings_Menu
  Settings_Menu.press((Cursor_Pressed || enterKeyPressed_Screen)&& Settings_Menu.contains(Cursor_X, Cursor_Y));
  if (Settings_Menu.justPressed()){
    enterKeyPressed_Screen = false;
    tabCount = 0;
    Settings_Menu.drawButton(false);
    Change_Menu(1);
  }

  Chat_Menu.press((Cursor_Pressed || enterKeyPressed_Screen) && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    enterKeyPressed_Screen = false;
    tabCount = 0;
    chatOffset = 0;
    selectedChat = -1;
    Chat_Menu.drawButton(false);
    Change_Menu(2);
  }

}


void Refresh_Settings_Menu() {

  if (tabKeyPressed) {

    tabKeyPressed = false;
    tabCount++;

    if (tabCount % 4 == 1) { // Back Button

    Cursor_X = 29;
    Cursor_Y = 42;

    Back_Button.drawButton(false);
    increase_Sound_Length_Button.drawButton(true);

    } else if (tabCount % 4 == 2) { // Sound Button

      Cursor_X = 225;
      Cursor_Y = 98;
      
      Back_Button.drawButton(true);
      sound_Turned_On_Button.drawButton(false);
      
    } else if (tabCount % 4 == 3) { // decrease sound duration Button

      Cursor_X = 120;
      Cursor_Y = 170;
      
      sound_Turned_On_Button.drawButton(true);
      decrease_Sound_Length_Button.drawButton(false);

    } else if (tabCount % 4 == 0) { // increase sound duration Button

      Cursor_X = 290;
      Cursor_Y = 170;
      
      decrease_Sound_Length_Button.drawButton(true);
      increase_Sound_Length_Button.drawButton(false);

    }
  }

  Back_Button.press((Cursor_Pressed|| enterKeyPressed_Screen) && Back_Button.contains(Cursor_X, Cursor_Y));
  if (Back_Button.justPressed() || escKeyPressed) {
    escKeyPressed = false;
    enterKeyPressed_Screen = false;
    tabCount = 0;
    Cursor_X = 0;
    Cursor_Y = 0;
    Back_Button.drawButton(false);
    Change_Menu(0);
  }

  sound_Turned_On_Button.press((Cursor_Pressed || enterKeyPressed_Screen) && sound_Turned_On_Button.contains(Cursor_X, Cursor_Y));
  if (sound_Turned_On_Button.justPressed()) {
    /*if (sound_Switch_Active) {
      sound_Switch_Active = false;
    } else {
      sound_Switch_Active = true;
    }*/
    enterKeyPressed_Screen = false;
    sound_Switch_Active = !sound_Switch_Active;
    sound_Turned_On_Button.drawButton(false);
    delay(100);
    Draw_Settings_Menu();
  } else if (soundOn || soundOff) {
    soundOn = false;
    soundOff = false;
    sound_Turned_On_Button.drawButton(false);
    delay(100);
    Draw_Settings_Menu();
  }

  decrease_Sound_Length_Button.press((Cursor_Pressed || enterKeyPressed_Screen) && decrease_Sound_Length_Button.contains(Cursor_X, Cursor_Y));
  if (decrease_Sound_Length_Button.justPressed() || decreaseSoundTime) {
    if(!decreaseSoundTime && soundLength > 0){
      soundLength--;
    }
    enterKeyPressed_Screen = false;
    decreaseSoundTime = false;

    decrease_Sound_Length_Button.drawButton(false);

    tft.fillRect(195, 145, 14, 14, WHITE);
    tft.setTextColor(BLACK);

    if(soundLength < 10){
    tft.setCursor(199, 145);
    tft.print(soundLength);
    } else if (soundLength == 10) {
      tft.setCursor(196, 145);
      tft.print(soundLength);
    }

    tft.fillRect(152, 165, 100, 10, LIGHTGREY);
    tft.fillRect(152, 165, soundLength*10, 10, TURQUOISE);
    tft.drawRect(153, 164, 100, 10, BLACK);
    tft.drawRect(152, 165, 100, 10, BLACK);
    tft.drawRect(151, 166, 100, 10, BLACK);

    delay(50);

    decrease_Sound_Length_Button.drawButton(true);

  }

  increase_Sound_Length_Button.press((Cursor_Pressed || enterKeyPressed_Screen) && increase_Sound_Length_Button.contains(Cursor_X, Cursor_Y));
  if (increase_Sound_Length_Button.justPressed() || increaseSoundTime) {
    if(!increaseSoundTime && soundLength < 10){
      soundLength++;
    }
    enterKeyPressed_Screen = false;
    increaseSoundTime = false;

    increase_Sound_Length_Button.drawButton(false);

    tft.fillRect(195, 145, 14, 14, WHITE);
    tft.setTextColor(BLACK);

    if(soundLength < 10){
    tft.setCursor(199, 145);
    tft.print(soundLength);
    } else if (soundLength == 10) {
      tft.setCursor(196, 145);
      tft.print(soundLength);
    }

    tft.fillRect(152, 165, 100, 10, LIGHTGREY);
    tft.fillRect(152, 165, soundLength*10, 10, TURQUOISE);
    tft.drawRect(153, 164, 100, 10, BLACK);
    tft.drawRect(152, 165, 100, 10, BLACK);
    tft.drawRect(151, 166, 100, 10, BLACK);


    delay(50);

    increase_Sound_Length_Button.drawButton(true);

  }

}


void Refresh_Chat_Menu() {

    if (refresh_Chat_Menu) {
      refresh_Chat_Menu = false;

      Draw_Chat_Menu();
      Refresh_Chat_Menu();
    }
    
    if (tabCount == 0 && enterKeyPressed_Screen && selectedChat == -1) {
      enterKeyPressed_Screen = false;
    }

    if (tabKeyPressed) {

      if (selectedChat >= 0) {
        chatButtons[selectedChat].drawButton(true);
      }

      tabKeyPressed = false;
      tabCount++;

      if (tabCount != 0 && tabCount % 4 == 1) { // Back Button

        Cursor_X = 29;
        Cursor_Y = 42;

        Back_Button.drawButton(false);
        New_Chat_Button.drawButton(true);
        scrollUpButton.drawButton(true);
        scrollDownButton.drawButton(true);
        
      } else if (tabCount != 0 && tabCount % 4 == 2) { // New Contact Button

        Cursor_X = 266;
        Cursor_Y = 42;

        Back_Button.drawButton(true);
        New_Chat_Button.drawButton(false);
        scrollUpButton.drawButton(true);
        scrollDownButton.drawButton(true);

      } else if (tabCount != 0 && tabCount % 4 == 3) { // Up Button

        Cursor_X = 290;
        Cursor_Y = 78;

        Back_Button.drawButton(true);
        New_Chat_Button.drawButton(true);
        scrollUpButton.drawButton(false);
        scrollDownButton.drawButton(true);

        
      } else if (tabCount != 0 && tabCount % 4 == 0) { // Down Button
        
        Cursor_X = 290;
        Cursor_Y = 221;

        Back_Button.drawButton(true);
        New_Chat_Button.drawButton(true);
        scrollUpButton.drawButton(true);
        scrollDownButton.drawButton(false);

      }

    }

    
    if (arrowDownPressed) { // Scroll one chat down

      if(tabCount !=0) {
        if (selectedChat >= 0) {
          chatButtons[selectedChat].drawButton(true);
        }
        tabCount = 0;
        selectedChat = -1;
        chatOffset = 0;
        Draw_Chat_Menu();
      }

      arrowDownPressed = false;

      if(selectedChat < 3) { // if Contacts 0 to 2 selected

        selectedChat++;

        if(selectedChat > 0) {
          Serial.println("in selectedChat != 0 case");

          chatButtons[(selectedChat - 1)].drawButton(true);
        }

        if(selectedChat > -1) {
          chatButtons[selectedChat].drawButton(false);
        }

        Cursor_X = 129;
        Cursor_Y = 70 + selectedChat * 40;

      } else if(selectedChat == chatCount - 1) { // if last Contact selected -> move to Contact 0
        Serial.println("in selectedChat == chatCount case");

        Cursor_X = 129;
        Cursor_Y = 70;

        selectedChat = 0;
        chatOffset = 0;

        Draw_Chat_Menu();

        chatButtons[3].drawButton(true);
        chatButtons[0].drawButton(false);

      } else if (selectedChat >= 3) { // if Contact 3 or higher selected, move screen one Contact down

        Cursor_X = 129;
        Cursor_Y = 190;

        Serial.println("in selectedChat >= 3 case");
        selectedChat++;
        chatOffset++;
        Draw_Chat_Menu();
        chatButtons[2].drawButton(true);
        chatButtons[3].drawButton(false);
      }

    }
    
    if (arrowUpPressed) {

      arrowUpPressed = false;

      if (tabCount != 0){
        if (selectedChat <= 0){
          chatButtons[selectedChat].drawButton(true);

          tabCount = 0;
          selectedChat = 0;
          chatOffset = 0;
          Draw_Chat_Menu();

          chatButtons[0].drawButton(false);

          Cursor_X = 129;
          Cursor_Y = 70;

        }
      }

      if (selectedChat == 0){ // if Contact 0 is selected
        selectedChat = chatCount - 1;
        chatOffset = chatCount - 4;

        Draw_Chat_Menu();

        chatButtons[0].drawButton(true);
        chatButtons[3].drawButton(false);

        Cursor_X = 129;
        Cursor_Y = 190;

      } else if (selectedChat < 0){

          tabCount = 0;
          selectedChat = 0;
          chatOffset = 0;
          Draw_Chat_Menu();

          chatButtons[0].drawButton(false);

          Cursor_X = 129;
          Cursor_Y = 70;


      } else if (selectedChat > (chatCount - 4)){ // if Contact(#chatCount-1) to Contact(#chatCount-3) is selected, only move Cursors
        
        selectedChat--;
        //chatOffset--; if not working try to implement this

        chatButtons[5 + selectedChat - chatCount].drawButton(true);
        chatButtons[4 + selectedChat - chatCount].drawButton(false);

        Cursor_X = 129;
        Cursor_Y = 190 - ((chatCount - selectedChat - 1) * 40);

      } else { // if Contact between (#chatCount-3) and 1, move up by one Contact

        if (chatOffset > 0) {
          chatOffset--;
        }

        selectedChat--;
        Draw_Chat_Menu();

        chatButtons[1].drawButton(true);
        chatButtons[0].drawButton(false);

        Cursor_X = 129;
        Cursor_Y = 70;

      }

    }
    
    Back_Button.press((Cursor_Pressed || enterKeyPressed_Screen) && Back_Button.contains(Cursor_X, Cursor_Y));
    if (Back_Button.justPressed() || escKeyPressed) {
        enterKeyPressed_Screen = false;
        escKeyPressed = false;
        Back_Button.drawButton(false);
        Change_Menu(0);
    }

    New_Chat_Button.press((Cursor_Pressed || enterKeyPressed_Screen) && New_Chat_Button.contains(Cursor_X, Cursor_Y));
    if (New_Chat_Button.justPressed()) {
        enterKeyPressed_Screen = false;
        New_Chat_Button.drawButton(false);
        Change_Menu(4);
    }

    scrollUpButton.press((Cursor_Pressed || enterKeyPressed_Screen) && scrollUpButton.contains(Cursor_X, Cursor_Y));
    if (scrollUpButton.justPressed()) {
        enterKeyPressed_Screen = false;
        arrowUpPressed = false;
        scrollUpButton.drawButton(false);
        if (chatOffset > 0) {
          chatOffset--; // Scroll up
          Draw_Chat_Menu();
          scrollUpButton.drawButton(false);
        } else if (chatOffset == 0) {
          chatOffset = chatCount - 4;
          Draw_Chat_Menu();
          scrollUpButton.drawButton(false);
        } 
    }

    scrollDownButton.press((Cursor_Pressed || enterKeyPressed_Screen) && scrollDownButton.contains(Cursor_X, Cursor_Y));
    if (scrollDownButton.justPressed()) {
        enterKeyPressed_Screen = false;
        arrowDownPressed = false;
        scrollDownButton.drawButton(false);
        if (chatOffset + 4 < chatCount) {
            chatOffset++; // Scroll down
            Draw_Chat_Menu(); // Redraw menu
            scrollDownButton.drawButton(false);
        } else if (chatOffset + 4 == chatCount) {
            chatOffset = 0;
            Draw_Chat_Menu();
            scrollDownButton.drawButton(false);
        }
    }

    // Check for button presses on visible chat buttons
    //uint8_t visibleChats = 4; // Number of chats visible at a time
    for (uint8_t i = 0; i < visibleChats && (i + chatOffset) < chatCount; i++) {
        chatButtons[i].press((Cursor_Pressed || enterKeyPressed_Screen) && chatButtons[i].contains(Cursor_X, Cursor_Y));   
        if (chatButtons[i].justPressed()) {
            chatButtons[i].drawButton(false);
            phoneNumber = chatList[i + chatOffset].phoneNumber;
            contactName = chatList[i + chatOffset].contactName;
            enterKeyPressed_Screen = false;
            Change_Menu(3); // Go to chat viewer
        }
  
    }

    enterKeyPressed_Screen = false;
  }


void Refresh_Chat_Viewer() {
  Back_Button.press(Cursor_Pressed && Back_Button.contains(Cursor_X, Cursor_Y));

  Blink_Underscore(Chat_Cursor_X, Chat_Cursor_Y);

  if (Back_Button.justPressed()){
    
    message = "";
    phoneNumber = "";
    contactName = "";

    tabCount = 0;
    chatOffset = 0;
    selectedChat = -1;

    message_Cursor_X = 4;
    message_Cursor_Y = 66;

    Back_Button.drawButton(false);
    Change_Menu(2);
  }

  scroll_Up_Chat_Button.press((Cursor_Pressed && scroll_Up_Chat_Button.contains(Cursor_X, Cursor_Y))  || arrowUpPressed);
  if (scroll_Up_Chat_Button.justPressed()) {

    arrowUpPressed = false;
    scroll_Up_Chat_Button.drawButton(false);

    if (startIndexChat < 0) {
      tft.fillRect(0, 56, 320, 169, WHITE);
      tft.drawLine(0, 56, 320, 56, BLACK);
      tft.drawLine(0, 225, 320, 225, BLACK);
      loadMessages(phoneNumber.c_str(), 0, getStoredMessagesCount(phoneNumber.c_str()));
      startIndexChat = 0;
    }

    if (startIndexChat > 0) {
      startIndexChat--;
      message_Cursor_X = 4;
      message_Cursor_Y = 66;
      tft.fillRect(0, 56, 320, 169, WHITE);
      tft.drawLine(0, 56, 320, 56, BLACK);
      tft.drawLine(0, 225, 320, 225, BLACK);
      loadMessages(phoneNumber.c_str(), startIndexChat, 4);
      Serial.println(startIndexChat);

    }

    delay(100);
    scroll_Up_Chat_Button.drawButton(true);

  }

  scroll_Down_Chat_Button.press((Cursor_Pressed && scroll_Down_Chat_Button.contains(Cursor_X, Cursor_Y)) || arrowDownPressed);
  if (scroll_Down_Chat_Button.justPressed()) {

    arrowDownPressed = false;
    scroll_Down_Chat_Button.drawButton(false);

    if (startIndexChat < getStoredMessagesCount(phoneNumber.c_str()) - 4) {
      startIndexChat++;
      message_Cursor_X = 4;
      message_Cursor_Y = 66;
      tft.fillRect(0, 56, 320, 169, WHITE);
      tft.drawLine(0, 56, 320, 56, BLACK);
      tft.drawLine(0, 225, 320, 225, BLACK);
      loadMessages(phoneNumber, startIndexChat, 4);
      Serial.println(startIndexChat);
    
    }

    delay(100);
    scroll_Down_Chat_Button.drawButton(true);
    
  }

  if (keyPressDetected){
    tft.setTextSize(1);
    tft.setTextColor(BLACK);
    if (lastKeyPressed.length() == 1) {
      Serial.println("in if statement");
      tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
      tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y + 7, 6, 2, WHITE);
      tft.print(lastKeyPressed);
      lastKeyPressed = "";
      Chat_Cursor_X += 6;
    } else if (lastKeyPressed.length() == 2){
      Serial.println("Doubleinput");
      tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
      tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y + 7, 6, 2, WHITE);
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
    enterKeyPressed_SMS = true;
    Chat_Cursor_X = 4;
    startIndexChat = getStoredMessagesCount(phoneNumber.c_str()) - 4;
    //refresh_Chat_View = true;
    delay(100);
    Draw_Chat_Viewer(phoneNumber.c_str(), contactName.c_str());
    //Refresh_Chat_Viewer();
  }
  
  if (escKeyPressed) {

    escKeyPressed = false;
    
    message = "";
    phoneNumber = "";
    contactName = "";

    tabCount = 0;
    chatOffset = 0;
    selectedChat = -1;

    message_Cursor_X = 4;
    message_Cursor_Y = 66;
    
    Back_Button.drawButton(false);
    Change_Menu(2);
  }

  if (refresh_Chat_View && recipientPhoneNumber == phoneNumber) { // add case if less than 4 messages
  
      refresh_Chat_View = false;
      
      if (getStoredMessagesCount(phoneNumber.c_str()) < 4) {

        message_Cursor_X = 4;
        message_Cursor_Y = 66;
        tft.fillRect(0, 56, 320, 169, WHITE);
        tft.drawLine(0, 56, 320, 56, BLACK);
        tft.drawLine(0, 225, 320, 225, BLACK);

        loadMessages(phoneNumber, 0, getStoredMessagesCount(phoneNumber.c_str()));

      } else {
        startIndexChat = getStoredMessagesCount(phoneNumber.c_str()) - 4;

        message_Cursor_X = 4;
        message_Cursor_Y = 66;
        tft.fillRect(0, 56, 320, 169, WHITE);
        tft.drawLine(0, 56, 320, 56, BLACK);
        tft.drawLine(0, 225, 320, 225, BLACK);

        loadMessages(phoneNumber, startIndexChat, 4);
      }

      //Refresh_Chat_Viewer();
  }
  
}

void Refresh_Phone_Number_Selector() {

    if (tabCount == 0 && enterKeyPressed_Screen) {
      enterKeyPressed_Screen = false;
    }
    if (tabKeyPressed && !keyboardActive){

      tabKeyPressed = false;
      tabCount++;

      if(tabCount != 0 && tabCount % 4 == 1){ // Back Button

        Cursor_X = 29;
        Cursor_Y = 42;

        Back_Button.drawButton(false);
        Enter_Button.drawButton(true);

        tft.drawRect(3, 79, 314, 47, WHITE);
        tft.drawRect(4, 80, 312, 45, BLACK);
        tft.drawRect(5, 81, 310, 43, WHITE);

        tft.drawRect(3, 159, 314, 47, WHITE);
        tft.drawRect(4, 160, 312, 45, BLACK);
        tft.drawRect(5, 161, 310, 43, WHITE);

      } else if (tabCount != 0 && tabCount % 4 == 2){ // Phone Number Window

        Cursor_X = 10;
        Cursor_Y = 90;

        Back_Button.drawButton(true);
        Enter_Button.drawButton(true);

        tft.drawRect(3, 79, 314, 47, TURQUOISE);
        tft.drawRect(4, 80, 312, 45, TURQUOISE);
        tft.drawRect(5, 81, 310, 43, TURQUOISE);

        tft.drawRect(3, 159, 314, 47, WHITE);
        tft.drawRect(4, 160, 312, 45, BLACK);
        tft.drawRect(5, 161, 310, 43, WHITE);


      } else if (tabCount != 0 && tabCount % 4 == 3){ // Contact Name Window

        Cursor_X = 10;
        Cursor_Y = 170;

        Back_Button.drawButton(true);
        Enter_Button.drawButton(true);

        tft.drawRect(3, 79, 314, 47, WHITE);
        tft.drawRect(4, 80, 312, 45, BLACK);
        tft.drawRect(5, 81, 310, 43, WHITE);

        tft.drawRect(3, 159, 314, 47, TURQUOISE);
        tft.drawRect(4, 160, 312, 45, TURQUOISE);
        tft.drawRect(5, 161, 310, 43, TURQUOISE);

      } else if (tabCount != 0 && tabCount % 4 == 0){ // Enter Button

        Cursor_X = 291;
        Cursor_Y = 42;

        Back_Button.drawButton(true);
        Enter_Button.drawButton(false);

        tft.drawRect(3, 79, 314, 47, WHITE);
        tft.drawRect(4, 80, 312, 45, BLACK);
        tft.drawRect(5, 81, 310, 43, WHITE);

        tft.drawRect(3, 159, 314, 47, WHITE);
        tft.drawRect(4, 160, 312, 45, BLACK);
        tft.drawRect(5, 161, 310, 43, WHITE);

      }
    }

    Back_Button.press((Cursor_Pressed || enterKeyPressed_Screen)&& Back_Button.contains(Cursor_X, Cursor_Y));
    if ((Back_Button.justPressed() || escKeyPressed) && !keyboardActive) {
        Back_Button.drawButton(false);
        enterKeyPressed_Screen = false;
        escKeyPressed = false;
        Chat_Cursor_X = 4;
        Chat_Cursor_Y = 230;
        Change_Menu(2); // Return to Chat Menu
    }

    // Check if Phone Number field is tapped
    if ((Cursor_Pressed || enterKeyPressed_Screen) && Cursor_X > 4 && Cursor_X < 316 && Cursor_Y > 80 && Cursor_Y < 125) {

        tft.drawRect(3, 79, 314, 47, TURQUOISE);
        tft.drawRect(4, 80, 312, 45, TURQUOISE);
        tft.drawRect(5, 81, 310, 43, TURQUOISE);

        tft.drawRect(3, 159, 314, 47, WHITE);
        tft.drawRect(4, 160, 312, 45, BLACK);
        tft.drawRect(5, 161, 310, 43, WHITE);

        tabCount = 1;
        enterKeyPressed_Screen = false;
        escKeyPressed = false;

        tft.drawRect(3, 159, 314, 47, WHITE);
        tft.drawRect(4, 160, 312, 45, BLACK);
        tft.drawRect(5, 161, 310, 43, WHITE);

        Serial.println("Bitte geben Sie die Telefonnummer ein:");

        tft.fillRect(5, 81, 310, 43, WHITE);  // Clear the field

        keyboardActive = true;

        tft.setCursor(10, 85);
        tft.print("Enter Phone Number:");

        Chat_Cursor_X = 10;
        Chat_Cursor_Y = 105;

        while(keyboardActive) {
          
          Blink_Underscore(Chat_Cursor_X, Chat_Cursor_Y);

          if (keyPressDetected){
            tft.setTextSize(1);
            tft.setTextColor(BLACK);
            if (lastKeyPressed.length() == 1) {
              Serial.println("in if statement");
              tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
              tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y + 7, 6, 2, WHITE);
              tft.print(lastKeyPressed);
              lastKeyPressed = "";
              Chat_Cursor_X += 6;
            } else if (lastKeyPressed.length() == 2){
              Serial.println("Doubleinput");
              //tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
              tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y + 7, 6, 2, WHITE);
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
            enterKeyPressed_Screen = false;
            keyboardActive = false;
            tft.fillRect(0, SCREEN_HEIGHT - 12 , SCREEN_WIDTH, 12, WHITE);
            tft.setTextSize(1);
            Chat_Cursor_X = 4;
            Chat_Cursor_Y = 230;
          }
          
          if (escKeyPressed) {
            escKeyPressed = false;
            Chat_Cursor_X = 4;
            Chat_Cursor_Y = 230;
            
            message = "";
            phoneNumber = "";
            contactName = "";

            tft.fillRect(4, 80, 312, 45, WHITE);
            tft.setTextSize(1);
            tft.setCursor(81, 95); // Adjusted Y-coordinate for centered text
            tft.print("Click to enter phone number");

            tft.drawRect(3, 79, 314, 47, TURQUOISE);
            tft.drawRect(4, 80, 312, 45, TURQUOISE);
            tft.drawRect(5, 81, 310, 43, TURQUOISE);

            keyboardActive = false;
          }

        }

        if (!escKeyPressed && phoneNumber.length() > 0) {
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
    if ((Cursor_Pressed || enterKeyPressed_Screen) && Cursor_X > 4 && Cursor_X < 316 && Cursor_Y > 160 && Cursor_Y < 205) {

        tft.drawRect(3, 79, 314, 47, WHITE);
        tft.drawRect(4, 80, 312, 45, BLACK);
        tft.drawRect(5, 81, 310, 43, WHITE);

        tft.drawRect(3, 159, 314, 47, TURQUOISE);
        tft.drawRect(4, 160, 312, 45, TURQUOISE);
        tft.drawRect(5, 161, 310, 43, TURQUOISE);

        tabCount = 2;
        enterKeyPressed_Screen = false;
        escKeyPressed = false;
        Serial.println("Bitte geben Sie den Kontaktnamen ein:");

        tft.drawRect(3, 79, 314, 47, WHITE);
        tft.drawRect(4, 80, 312, 45, BLACK);
        tft.drawRect(5, 81, 310, 43, WHITE);

        tft.fillRect(5, 161, 310, 43, WHITE); // Clear the field

        keyboardActive = true; // Enter to confirm -> deactivate keyboard again

        tft.setCursor(10, 165);
        tft.print("Enter Contact Name:");

        Chat_Cursor_X = 10;
        Chat_Cursor_Y = 185;

        while(keyboardActive) {

          Blink_Underscore(Chat_Cursor_X, Chat_Cursor_Y);

          if (keyPressDetected){

            tft.setTextSize(1);
            tft.setTextColor(BLACK);

            if (lastKeyPressed.length() == 1) {

              Serial.println("in if statement");
              tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
              tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y + 7, 6, 2, WHITE);
              tft.print(lastKeyPressed);
              lastKeyPressed = "";
              Chat_Cursor_X += 6;

            } else if (lastKeyPressed.length() == 2){

              Serial.println("Doubleinput");
              tft.setCursor(Chat_Cursor_X, Chat_Cursor_Y);
              tft.fillRect(Chat_Cursor_X, Chat_Cursor_Y + 7, 6, 2, WHITE);
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
            escKeyPressed = false;
            Chat_Cursor_X = 4;
            Chat_Cursor_Y = 230;
            
            message = "";
            phoneNumber = "";
            contactName = "";

            tft.fillRect(4, 160, 312, 45, WHITE);
            tft.setTextSize(1);
            tft.setCursor(81, 175); // Adjusted Y-coordinate for centered text
            tft.print("Click to enter contact name");

            tft.drawRect(3, 159, 314, 47, TURQUOISE);
            tft.drawRect(4, 160, 312, 45, TURQUOISE);
            tft.drawRect(5, 161, 310, 43, TURQUOISE);

            keyboardActive = false;
            
          }
        }

        if(!escKeyPressed && contactName.length() > 0) {
          escKeyPressed = false;
          tft.fillRect(5, 161, 310, 43, WHITE); // Clear the field
          tft.setCursor(18, 175);
          tft.setTextSize(2);

          tft.print(contactName); // Display entered contact name on the screen
          Serial.print("Eingegebener Kontaktname: ");
          Serial.println(contactName); // Debugging output
          tft.setTextSize(1);
        }
    }

    // Check if Enter Button is pressed
    Enter_Button.press((Cursor_Pressed || enterKeyPressed_Screen) && Enter_Button.contains(Cursor_X, Cursor_Y));
    if (Enter_Button.justPressed() || enterKeyPressed_Screen) {
        enterKeyPressed_Screen = false;
        Enter_Button.drawButton(false);

        // Ensure both fields are filled
      if (phoneNumber.length() > 0 && contactName.length() > 0) {
              storeContact(phoneNumber.c_str(), contactName.c_str()); // Create new contact
              newContact(phoneNumber, contactName);
              Serial.println("Neuer Kontakt erfolgreich erstellt:");
              Serial.print("Telefonnummer: ");
              Serial.println(phoneNumber);
              Serial.print("Kontaktname: ");
              Serial.println(contactName);
      } else { // Error Window for missing Phone Number/Contact Name
          
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

  escKeyPressed= false;

}

// Change Menus --------------------------------------------------------------------------------------------
void Change_Menu(uint8_t no){

    tabCount = 0;
    tabKeyPressed = false;
    enterKeyPressed_Screen = false;
    escKeyPressed = false;
    arrowUpPressed = false;
    arrowDownPressed = false;

    message_Cursor_X = 4;
    message_Cursor_Y = 66;


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
        if (getStoredMessagesCount(phoneNumber.c_str()) < 4) {
          startIndexChat = getStoredMessagesCount(phoneNumber.c_str());
        } else {
          startIndexChat = getStoredMessagesCount(phoneNumber.c_str())-4;
        }
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

  arrowUpPressed = false;
  arrowDownPressed = false;
  message_Cursor_X = 4;
  message_Cursor_Y = 66;

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

  tft.fillRect(x + 6, y + 7, 8, 2, WHITE);

  if (millis() - lastBlink > 500) { // Toggle every 500ms
    lastBlink = millis();
    underscoreVisible = !underscoreVisible;
    tft.fillRect(x, y + 7, 6, 2, WHITE);
          
  }

  if (underscoreVisible) {
    tft.fillRect(x, y + 7, 6, 2, BLACK);
  }
    
}
