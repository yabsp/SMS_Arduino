// Libraries -------------------------------------------------------------------
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <FreeDefaultFonts.h>

// Global Definitions ----------------------------------------------------------
#define MINPRESSURE 200
#define MAXPRESSURE 1000
#define MAX_CHATS 5
#define MAX_MESSAGES 10

// Global Variables ------------------------------------------------------------
const int XP=8,XM=A2,YP=A3,YM=9; // 320x240 ID=0x9341
const int TS_LEFT = 27, TS_RT = 907, TS_TOP = 101, TS_BOT = 932;
int Cursor_X, Cursor_Y;
bool Cursor_Pressed;
int currentChatIndex = 0;

// Create Objects --------------------------------------------------------------
MCUFRIEND_kbv tft; // create display object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // create touchscreen object
struct Chat {
  String name;
  String messages[MAX_MESSAGES];
  int messageCount;
};
Chat chatWindows[MAX_CHATS] = {
  {"Alice", {"Hello!", "How are you?"}, 2},
  {"Bob", {"Hey there!", "What's up?"}, 2},
  {"Charlie", {"Hi!", "Long time no see!"}, 2},
  {"Diana", {"Good morning!"}, 1},
  {"Eve", {}, 0}
};

// Function to get touch coordinates -------------------------------------------
bool Touch_getXY(void) {
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      // restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   // because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        Cursor_X = map(p.y, TS_LEFT, TS_RT, 0, tft.width());
        Cursor_Y = map(p.x, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

// Colors ----------------------------------------------------------------------
#define BLACK      0x0000
#define BLUE       0x001F
#define RED        0xF800
#define GREEN      0x07E0
#define CYAN       0x07FF
#define MAGENTA    0xF81F
#define YELLOW     0xFFE0
#define WHITE      0xFFFF
#define TURQUOISE  0x4639
#define LIGHTGRAY  0xC618
#define DARKGRAY   0x7BEF

// Buttons ---------------------------------------------------------------------
Adafruit_GFX_Button New_Message, Contact_Menu, Ringtone_Menu;
Adafruit_GFX_Button Main_Menu, Chat_Menu, Settings_Menu;
Adafruit_GFX_Button ChatButtons[MAX_CHATS];

uint8_t Current_Menu;
void Change_Menu(uint8_t);
void Refresh_Menu();
int32_t Position_X;
int32_t Position_Y;

int calculateLeft(int width) {
  return width / 2;
}
int calculateRight(int width) {
  return width / 2 - width;
}

int calculateTop(int height) {
  return height / 2;
}

int calculateBottom(int height) {
  return height / 2;
}

void createChatButtons(int chatAmount) {
    // Ensure chatAmount does not exceed MAX_CHATS
    if (chatAmount > MAX_CHATS) chatAmount = MAX_CHATS;

    for (int i = 0; i <= chatAmount; i++) {
      char label[20]; // Buffer for the label
      snprintf(label, sizeof(label), "Chat %d", i + 1); // Format "Chat 1", "Chat 2", etc.
      ChatButtons[i].initButton(
        &tft,
        calculateLeft(210) + 10, // X position
        calculateTop(20)+ (i+1) * 30, // Y position
        220, 20,
        BLACK, BLACK, WHITE, 
        label, 
        1);
    }
}

// Setup -----------------------------------------------------------------------
void setup() {
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  tft.setFont(NULL);
  // Buttonname.initButton(&tft, Start X rel., Start Y rel., Width, Height, Outline, Text, Fill, String, TextSize);
  // Main_Menu
    Settings_Menu.initButton(&tft, calculateLeft(145)+10, calculateTop(20) + 170, 145, 25, BLACK, BLACK, WHITE, "Settings", 1);
    Chat_Menu.initButton(&tft, calculateRight(145)+310, calculateTop(20) + 170, 145, 25, BLACK, BLACK, WHITE, "Chats", 1);

  // Chat_Menu
    New_Message.initButton(&tft, calculateRight(80)+310, calculateTop(20) + 30, 80, 20, BLACK, BLACK, WHITE, "New Chat", 1);
    Contact_Menu.initButton(&tft, calculateRight(80)+310, calculateTop(20) + 90, 80, 20, BLACK, BLACK, WHITE, "Contacts", 1);
    /*createChatButtons(chatAmount);*/

  // Settings_Menu
    Ringtone_Menu.initButton(&tft, calculateRight(80)+310, calculateTop(20) + 60, 80, 20, BLACK, BLACK, WHITE, "Ringtones", 1);

  Change_Menu(0); // Start in Chat Menu
}

// Show Main_Menu ----------------------------------------------------------------
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

// Handle clicks in Main_Menu
void Refresh_Main_Menu() {
  // Chat_Menu
  Chat_Menu.press(Cursor_Pressed && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    Chat_Menu.drawButton(false);
    Change_Menu(1);
  }
  // Settings_Menu
  Settings_Menu.press(Cursor_Pressed && Settings_Menu.contains(Cursor_X, Cursor_Y));
  if (Settings_Menu.justPressed()){
    Settings_Menu.drawButton(false);
    Change_Menu(3);
  }
}

// Show Ringtone_Menu -----------------------------------------------------------------
void Draw_Ringtone_Menu() {
  // Background and Text
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 15);
  tft.print("Ringtonemenu");
  // Buttons
  Chat_Menu.drawButton(true);
}

// Handle Ringtone_Menu
void Refresh_Ringtone_Menu() {
  // Chat_Menu
  Main_Menu.press(Cursor_Pressed && Main_Menu.contains(Cursor_X, Cursor_Y));
  if (Main_Menu.justPressed()){
    Main_Menu.drawButton(false);
    Change_Menu(0);
  }
}

// Show Chat_Menu --------------------------------------------------------------
void Draw_Chat_Menu(){
  // Menubar
  tft.fillScreen(WHITE);
  tft.fillRect(0, 0, tft.width(), 20, TURQUOISE);
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  tft.setCursor(6, 6);
  tft.print("Basel, 30");
  tft.write(176); // Degree symbol
  tft.print("C"); // Prints "25°C"  
  tft.setCursor(215, 6);
  tft.print("22.12.2024, 11:46");


  for (int i = 0; i < MAX_CHATS; i++) {
      int y = 30 + i * 30;
      tft.fillRect(0, y, tft.width(), 30, LIGHTGRAY);
      tft.setCursor(10, y + 10);
      tft.setTextSize(1);
      tft.print(chatWindows[i].name);
  }
  // Buttons
  New_Message.drawButton(true);
  Contact_Menu.drawButton(true);
  Ringtone_Menu.drawButton(true);
  
}

// Handle clicks in Chat_Menu
void Refresh_Chat_Menu(){
    if (Cursor_Pressed) {
    for (int i = 0; i < MAX_CHATS; i++) {
      int y = 30 + i * 30;
      if (Cursor_Y > y && Cursor_Y < y + 30) {
        currentChatIndex = i;
        Draw_Chat_Window();
        break;
      }
    }
  }
  // New_Message
  New_Message.press(Cursor_Pressed && New_Message.contains(Cursor_X, Cursor_Y));
  if (New_Message.justPressed()){
    New_Message.drawButton(false);
    Change_Menu(2);
  }
  // Ringtone_Menu
  Ringtone_Menu.press(Cursor_Pressed && Ringtone_Menu.contains(Cursor_X, Cursor_Y));
  if (Ringtone_Menu.justPressed()){
    Ringtone_Menu.drawButton(false);
    Change_Menu(3);
  }
  // Contact_Menu
  Contact_Menu.press(Cursor_Pressed && Contact_Menu.contains(Cursor_X, Cursor_Y));
  if (Contact_Menu.justPressed()){
    Contact_Menu.drawButton(false);
    Change_Menu(4);
  }
}

// Show Chat_Window --------------------------------------------------------------
void Draw_Chat_Window() {
  // Background and Text
  tft.fillScreen(WHITE);
  tft.fillRect(0, 0, tft.width(), 18, TURQUOISE);
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  tft.setCursor(5, 5);
  tft.print("Basel, 30");
  tft.write(176); // Degree symbol
  tft.print("C"); // Prints "25°C"  
  tft.setCursor(215, 6);
  tft.print("22.12.2024, 11:46");


  tft.setTextSize(2);
  tft.setCursor(28, 15);

  tft.print("Chatwindow");

  Chat &chat = chatWindows[currentChatIndex];
  tft.fillScreen(WHITE);

  // Header
  tft.fillRect(0, 0, tft.width(), 20, TURQUOISE);
  tft.setTextColor(BLACK);
  tft.setCursor(6, 6);
  tft.print(chat.name);

  // Messages
  for (int i = 0; i < chat.messageCount; i++) {
    int y = 30 + i * 20;
    tft.setCursor(10, y);
    tft.setTextSize(1);
    tft.print(chat.messages[i]);
  }

  // Input Box
  tft.fillRect(0, tft.height() - 30, tft.width(), 30, LIGHTGRAY);
  tft.setCursor(10, tft.height() - 20);
  tft.setTextColor(DARKGRAY);
  tft.print("Type a message...");
}

// Handle clicks in Chat_Window
void Refresh_Chat_Window() {
    if (Cursor_Pressed && Cursor_Y > tft.height() - 30) {
    Chat &chat = chatWindows[currentChatIndex];
    if (chat.messageCount < MAX_MESSAGES) {
      chat.messages[chat.messageCount++] = "New Message!"; // Simulate typing
      Draw_Chat_Window();
    }
  }

  Chat_Menu.press(Cursor_Pressed && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    Chat_Menu.drawButton(false);
    Change_Menu(1);
  }
}

// Show Contact_Menu -----------------------------------------------------------------
void Draw_Contact_Menu() {
  // Background and Text
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 30);
  tft.print("Contactmenu");
  // Buttons
  Chat_Menu.drawButton(true);
}

// Handle clicks in Contact_Menu
void Refresh_Contact_Menu() {
  // Chat_Menu
  Chat_Menu.press(Cursor_Pressed && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    Chat_Menu.drawButton(false);
    Change_Menu(1);
  }
}

// Change menus -----------------------------------------------------------------
void Change_Menu(uint8_t no){
    Current_Menu = no;
    switch(no){ // change to the drawn menu
      case 0:
        Draw_Main_Menu();
        break;
      case 1:
        Draw_Chat_Menu();
        break;
      case 2:
        Draw_Chat_Window();
        break;
      case 3:
        Draw_Ringtone_Menu();
        break;
      case 4:
        Draw_Contact_Menu();
        break;
    }
}

// Refresh menus ----------------------------------------------------------------
void Refresh_Menu(){
  switch(Current_Menu){ // refesh the current menu
    case 0:
      Refresh_Main_Menu();
      break;
    case 1:
      Refresh_Chat_Menu();
      break;
    case 2:
      Refresh_Chat_Window();
      break;
    case 3:
      Refresh_Ringtone_Menu();
      break;
    case 4:
      Refresh_Contact_Menu();
      break;
  }
}

// Main loop -------------------------------------------------------------------
void loop() {
    Cursor_Pressed = Touch_getXY();
    Refresh_Menu(); // refresh on press
}