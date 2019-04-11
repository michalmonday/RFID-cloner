#ifndef NAMEPICKER_H
#define NAMEPICKER_H

#include <Arduino.h>

#include "Files.h"


#define NOT_READY 255
#define CANCELLED 254
#define READY 253

#include "Buttons.h"
extern Buttons buttons;

#include <functional>

class NamePicker
{
public:
  void Reset();
  void HandleControls();
  String GetName();
  int GetXpos(){return name_picker_x_pos;}
  void OnCancel(std::function<void()> func){onCancelFunc=func;}
  void OnSuccess(std::function<void()> func){onSuccessFunc=func;}

  void Update();
  void SetInitialName(String s){SetInitialName((char*)s.c_str());}
  void SetInitialName(char* s){strcpy(file_name, s);}

private:
  char file_name[MAX_FILENAME] = {0};
  int name_picker_x_pos = 0;
  int nameStatus = NOT_READY;

  std::function<void()> onCancelFunc = [](){};
  std::function<void()> onSuccessFunc = [](){};

  String charset = " abcdefghijklmnopqrstuvwxyz0123456789%[]()@^_`'!\"#$&\\*+,-./:;<=>?{|}~ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int char_index = 1;
};























#endif
