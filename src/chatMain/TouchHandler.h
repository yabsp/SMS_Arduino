#ifndef TOUCHHANDLER_H
#define TOUCHHANDLER_H

#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include "Config.h"

// Global variables ---------------------------------------------------------------------------
extern TouchScreen ts;

// Function prototyping -----------------------------------------------------------------------
bool Touch_getXY(int &Cursor_X, int &Cursor_Y);
void InitializeTouchscreen(MCUFRIEND_kbv &tft);

#endif