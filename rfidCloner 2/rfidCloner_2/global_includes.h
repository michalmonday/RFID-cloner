//#include "global_includes.h"

#ifndef GLOBAL_INCLUDES_H
#define GLOBAL_INCLUDES_H

#include <Arduino.h>

#define INVALID 255
#define CANCELLED 254

struct XY{
  int x;
  int y;
};

int GetNewLineCount(String s);
int GetNewLineCount(char * s);
String FormatBytes(unsigned long i);

#endif
