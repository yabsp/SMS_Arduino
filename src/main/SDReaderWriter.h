#ifndef SDREADERWRITER_H
#define SDREADERWRITER_H

#define CHIPSELECT 53

#include <SPI.h>
#include <SdFat.h>

extern SdFat SD;

void setupSDAndFolderStruct();
void storeMessage(const char*, const char*, const char*);
void storeContact(const char*, const char*);
void loadMessages(String, int, int);
void loadContacts();

#endif