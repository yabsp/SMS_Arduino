#include "KeyboardVariables.h"

bool altgrActive = false;
bool shiftActive = false;
bool capsActive = false;

unsigned long shiftLastTime = 0;
unsigned long altgrLastTime = 0;

const unsigned long timeout = 200;
unsigned long currentTime = 0;

uint16_t scanval = 0;
uint8_t lastscan = 0xE0;
uint8_t lastlastscan = 0xE0;
char input = '\0';
