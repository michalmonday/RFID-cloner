#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <Arduino.h>
#include <functional>

#include "buttons.h"
extern Buttons buttons;



class ProgressBar
{
public:
  void Update();
  void Reset();
  void SetLabel(String s){label = s;}
  void SetBorders(int lower, int higher){low_border = lower; high_border = higher;}
  
  void ResetLabel(){label = "";}
  void OnSuccess(std::function<void()> func){onSuccessFunc = func;}
  void OnFail(std::function<void()> func){onFailFunc = func;}
  void SetValue(int v){current_value = v;}
  void SetValue(String v){current_value = v.toInt();}
  void SetValGetter(std::function<int(int)> func){getValFunc = func; current_value = getValFunc(0);};
  
  int GetValue(){return current_value;}
  int GetProgressPercent(){return 100 - int((float(high_border) - float(current_value)) / (float(high_border) - float(low_border)) * 100);}
  String GetLabel(){return label;}
  int GetHigherBorder(){return high_border;}
  int GetLowerBorder(){return low_border;}
  
private:
  String label = "";
  int high_border = 100;
  int low_border = 0;
  int current_value = 0;

  std::function<void()> onSuccessFunc = [](){};
  std::function<void()> onFailFunc = [](){};
  std::function<int(int)> getValFunc = [](int i){return 0;};
};























#endif
