#include "TouchHandler.h"

// Function to get touch coordinates -------------------------------------------
bool Touch_getXY(int &Cursor_X, int &Cursor_Y) {
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        Cursor_X = map(p.y, TS_LEFT, TS_RT, 0, tft.width());
        Cursor_Y = map(p.x, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}


// Function to get setup touchscreen -------------------------------------------
void InitializeTouchscreen(MCUFRIEND_kbv &tft) {
  uint16_t ID = tft.readID(); 
  tft.begin(ID);
  tft.setRotation(1);
  tft.setFont(NULL);
}