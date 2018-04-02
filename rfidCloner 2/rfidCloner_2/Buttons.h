#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 4
#define BUTTON_YES 5
#define BUTTON_NO 6

#define STATE_UP 0
#define STATE_DOWN 1

  
class Buttons
{
public: 
  Buttons(){};
  
  void Update();
  int GetLast(){return last_button_pressed;}
  int GetLast(bool use_default_limiter){if(use_default_limiter){return Limiter() ? GetLast() : 0;}else{return GetLast();}}

  
  int GetHoldingTime(){return last_button_pressed > 0 ? millis() - last_button_pressed_time : 0;}
  unsigned long GetLastPressTime(){return last_button_pressed_time;}

  bool Limiter(int normal_speed_limit=250, int hold_speed_limit=125, int threshold=600);

  //void ForceRelease(){
  //  last_button_pressed = 0;
  //}
  
private:
  int last_button_pressed = 0;
  unsigned long last_button_pressed_time = 0;

  unsigned long last_button_passed_limiter_time = 0;
  int last_button_passed_limiter = last_button_passed_limiter;
};













#endif
