#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "Config.h"
#include "Utils.h"
#include "ChatHandler.h"
#include "sim7600g-h.h"

// Function prototyping -----------------------------------------------------------------------
void Draw_Main_Menu();
void Draw_Settings_Menu();
void Draw_Chat_Menu();
void Draw_Chat_Viewer(String phoneNumber, String contactName);
void Draw_Chat_Messages(const String& phoneNumber, const String& contactName);
void Draw_Phone_Number_Selector(String phoneNumber, String contactName);

void Refresh_Main_Menu();
void Refresh_Settings_Menu();
void Refresh_Chat_Menu();
void Refresh_Chat_Viewer();
void Refresh_Phone_Number_Selector();

void Change_Menu(uint8_t no);
void Refresh_Menu();

void InitializeButtons(MCUFRIEND_kbv &tft);

void Blink_Underscore(uint16_t x, uint16_t y);

#endif
