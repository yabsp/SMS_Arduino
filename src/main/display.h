#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <SD.h>

#include "Config.h"
#include "Menu.h"
#include "TouchHandler.h"

extern int Cursor_X;
extern int Cursor_Y;
extern bool Cursor_Pressed;
extern uint8_t Current_Menu;
extern int testing;

void setupDisplay();
void loopDisplay();

#endif