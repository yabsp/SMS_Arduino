#ifndef SDREADERWRITER_H
#define SDREADERWRITER_H

#define CHIPSELECT 53

#include <SPI.h>
#include <SdFat.h>
#include "Config.h"

extern SdFat SD;
extern uint16_t message_Cursor_X;
extern uint16_t message_Cursor_Y;

void setupSDAndFolderStruct();
void storeMessage(const char*, const char*, const char*);
void storeContact(const char*, const char*);
void loadMessages(String, int, int);
void loadContacts();
String getNameByPhoneNumber(const char*);
int getStoredMessagesCount(const char*);

#endif