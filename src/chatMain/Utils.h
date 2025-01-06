#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "Config.h"

// Function prototyping -----------------------------------------------------------------------
int calculateLeft(int width);
int calculateRight(int width);
int calculateTop(int height);
int calculateBottom(int height);
String buildStringFromInput();


#endif
