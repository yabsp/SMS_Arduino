// Libraries -------------------------------------------------------------------
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <FreeDefaultFonts.h>

// Global Definitions ----------------------------------------------------------
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Global Variables ------------------------------------------------------------
const int XP=8,XM=A2,YP=A3,YM=9; // 320x240 ID=0x9341
const int TS_LEFT = 128, TS_RT = 913, TS_TOP = 97, TS_BOT = 909;
int Cursor_X, Cursor_Y;
bool Cursor_Pressed;
int chatAmount = 9;
#define MAX_CHATS 10

// Objects ---------------------------------------------------------------------
MCUFRIEND_kbv tft; // create display object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // create touchscreen object

// Function to get touch coordinates -------------------------------------------
bool Touch_getXY(void) {
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      // restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   // because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        Cursor_X = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
        Cursor_Y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
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

// Buttons ---------------------------------------------------------------------
Adafruit_GFX_Button New_Message, Contact_Menu, Ringtone_Menu;
Adafruit_GFX_Button Main_Menu, Chat_Menu, Settings_Menu;
Adafruit_GFX_Button ChatButtons[MAX_CHATS];

uint8_t Current_Menu;
void Change_Menu(uint8_t);
void Refresh_Menu();

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
    // (Display, Start X rel., Start Y rel., Width, Height)
    New_Message.initButton(&tft,  calculateRight(80)+310, calculateTop(20) + 30, 80, 20, BLACK, BLACK, WHITE, "New Chat", 1);
    Ringtone_Menu.initButton(&tft,  calculateRight(80)+310, calculateTop(20) + 60, 80, 20, BLACK, BLACK, WHITE, "Ringtones", 1);
    Contact_Menu.initButton(&tft,  calculateRight(80)+310, calculateTop(20) + 90, 80, 20, BLACK, BLACK, WHITE, "Contacts", 1);
    Settings_Menu.initButton(&tft, calculateLeft(145)+0, calculateTop(20) + 170, 145, 20, BLACK, BLACK, WHITE, "Settings", 1);
    Chat_Menu.initButton(&tft, calculateRight(145)+310, calculateTop(20) + 170, 145, 18, BLACK, BLACK, WHITE, "Chats", 1);
    createChatButtons(chatAmount);
    Change_Menu(0);
}

void Draw_Main_Menu() {
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
  Settings_Menu.drawButton(true);
  Chat_Menu.drawButton(true);
}

void Refresh_Main_Menu() {
  Chat_Menu.press(Cursor_Pressed && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    Chat_Menu.drawButton(false);
    Change_Menu(1);
  }
}
   

// draw chat menu --------------------------------------------------------------
void Draw_Chat_Menu(){
  // menubar
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

  // buttons
  New_Message.drawButton(true);
  Contact_Menu.drawButton(true);
  Ringtone_Menu.drawButton(true);
  for(int i = 0; i < chatAmount; i++){
    ChatButtons[i].drawButton(true); // draw button on screen
  }

}

// refresh chat menu --------------------------------------------------------------
void Refresh_Chat_Menu(){
  tft.fillRect(75, 3, tft.width()- 2*75, 55, BLUE);
  New_Message.press(Cursor_Pressed && New_Message.contains(Cursor_X, Cursor_Y));
  if (New_Message.justPressed()){
    New_Message.drawButton(false);
    Change_Menu(2);
  }
  

  Ringtone_Menu.press(Cursor_Pressed && Ringtone_Menu.contains(Cursor_X, Cursor_Y));
  if (Ringtone_Menu.justPressed()){
    Ringtone_Menu.drawButton(false);
    Change_Menu(3);
  }

  Contact_Menu.press(Cursor_Pressed && Contact_Menu.contains(Cursor_X, Cursor_Y));
  if (Contact_Menu.justPressed()){
    Contact_Menu.drawButton(false);
    Change_Menu(4);
  }
}

// draw chat window --------------------------------------------------------------
void Draw_Chat_Window() {
  // menubar
  tft.fillScreen(WHITE);
  tft.fillRect(0, 0, tft.width(), 18, TURQUOISE);
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  tft.setCursor(5, 5);
  tft.print("Basel, 30");
  tft.write(176); // Degree symbol
  tft.print("C"); // Prints "25°C"  
  tft.setTextSize(2);
  tft.setCursor(20, 15);
  // chatwindo
  tft.print("Chatwindow");
  Chat_Menu.drawButton(true);
}

void Refresh_Chat_Window() {
  Chat_Menu.press(Cursor_Pressed && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    Chat_Menu.drawButton(false);
    Change_Menu(1);
  }
}

void Draw_Contacts_Menu() {
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 15);
  tft.print("Contactmenu");
  Chat_Menu.drawButton(true);
}
void Refresh_Contacts_Menu() {
  Chat_Menu.press(Cursor_Pressed && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    Chat_Menu.drawButton(false);
    Change_Menu(1);
  }
}

void Draw_Ringtone_Menu() {
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 15);
  tft.print("Ringtonemenu");
  Chat_Menu.drawButton(true);
}
void Refresh_Ringtone_Menu() {
  Chat_Menu.press(Cursor_Pressed && Chat_Menu.contains(Cursor_X, Cursor_Y));
  if (Chat_Menu.justPressed()){
    Chat_Menu.drawButton(false);
    Change_Menu(1);
  }
}

// change menus -----------------------------------------------------------------
void Change_Menu(uint8_t no){
    Current_Menu = no;
    switch(no){ // change to the drawn menu
      case 0:
        Draw_Main_Menu();
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
        Draw_Contacts_Menu();
        break;
    }
}


// refresh menus ----------------------------------------------------------------
void Refresh_Menu(){
  switch(Current_Menu){ // refesh the current menu
    case 0:
      Refresh_Main_Menu();
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
      Refresh_Contacts_Menu();
      break;
  }
}

//  loop -------------------------------------------------------------------
void loop() {
    Cursor_Pressed = Touch_getXY();
          Refresh_Menu(); // refresh on press
}