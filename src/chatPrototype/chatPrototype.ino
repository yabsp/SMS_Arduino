#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

// Define touch screen pins and parameters
#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define TS_MINX 100
#define TS_MAXX 920
#define TS_MINY 70
#define TS_MAXY 900
#define MINPRESSURE 10
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

MCUFRIEND_kbv tft;

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define WHITE   0xFFFF
#define BUTTON_COLOR BLUE
#define TEXT_COLOR WHITE

enum State { CHAT_SELECTION, CHAT_VIEW };
State currentState = CHAT_SELECTION;

const char* chats[] = { "Chat 1", "Chat 2", "Chat 3" };
const char* messages[][3] = {
  { "Hello!", "How are you?", "Goodbye!" },
  { "Hi!", "What's up?", "See you!" },
  { "Hey!", "Long time!", "Take care!" }
};
int selectedChat = -1;

void setup() {
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID();

  if (identifier == 0x0 || identifier == 0xFFFF) {
    Serial.println("LCD not detected. Forcing driver.");
    identifier = 0x9341; // Common driver
  }
  tft.begin(identifier);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  drawChatSelection();
}

void loop() {
  TSPoint p = ts.getPoint();

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // Map touch coordinates to screen dimensions
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);

    Serial.print("Touch X: ");
    Serial.println(p.x);
    Serial.print("Touch Y: ");
    Serial.println(p.y);

    if (currentState == CHAT_SELECTION) {
      handleChatSelection(p.x, p.y);
    } else if (currentState == CHAT_VIEW) {
      handleChatView(p.x, p.y);
    }
  }
}

// Draw the chat selection screen
void drawChatSelection() {
  Serial.println("Drawing Chat Selection Screen...");
  tft.fillScreen(BLACK);
  for (int i = 0; i < 3; i++) {
    tft.fillRect(10, 20 + i * 60, 220, 50, BUTTON_COLOR); //Rectangle
    tft.drawRect(10, 20 + i * 60, 220, 50, WHITE); //Outline
    tft.setCursor(20, 30 + i * 60);
    tft.setTextColor(TEXT_COLOR);
    tft.setTextSize(2);
    tft.print(chats[i]);
  }
}

// Handle chat selection
void handleChatSelection(int x, int y) {
  // Print touch coordinates for debugging
  Serial.print("Handling chat selection at X=");
  Serial.print(x);
  Serial.print(", Y=");
  Serial.println(y);
  int buttonYStart = 10;
  int buttonYEnd = 220;

  for (int i = 0; i < 3; i++) {
    int buttonXStart = 20 + i * 60;
    int buttonXEnd = buttonXStart + 50;
    

    // Check if touch is within the button's Y range
    if ((x > buttonXStart && x < buttonXEnd) && (y > buttonYStart && y < buttonYEnd)) {
      selectedChat = i;
      Serial.print("Selected Chat: ");
      Serial.println(selectedChat);
      drawChatView();
      currentState = CHAT_VIEW;
      return;
    }
  }

  Serial.println("No chat selected.");
}

// Draw the chat view screen
void drawChatView() {
  Serial.println("Drawing Chat View Screen...");
  if (selectedChat < 0 || selectedChat > 2) {
    Serial.println("Invalid Chat Selected!");
    return;
  }

  Serial.println("Chat Selected!");
  tft.fillScreen(BLACK);
  Serial.println("Painted Screen Black!");
  // Draw escape button
  tft.fillRect(10, 10, 50, 30, RED);
  tft.drawRect(10, 10, 50, 30, WHITE); // Outline for the button
  tft.setTextColor(TEXT_COLOR);
  tft.setCursor(15, 15);
  tft.setTextSize(1);
  tft.print("Back");

  // Display messages
  tft.setTextSize(2);
  tft.setTextColor(TEXT_COLOR);
  for (int i = 0; i < 3; i++) {
    tft.setCursor(10, 50 + i * 40);
    tft.print(messages[selectedChat][i]);
  }
}

// Handle chat view interactions
void handleChatView(int x, int y) {
  // Check if escape button is pressed
  if (x > 10 && x < 60 && y > 10 && y < 40) {
    Serial.println("Returning to Chat Selection...");
    currentState = CHAT_SELECTION;
    drawChatSelection();
  }
}
