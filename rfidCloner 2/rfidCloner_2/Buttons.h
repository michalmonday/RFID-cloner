#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include <functional>


#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 4
#define BUTTON_YES 5
#define BUTTON_NO 6

#define BUTTON_COUNT 6

#define STATE_UP 0
#define STATE_DOWN 1


struct Calibrator{
  int button;
  int frame_count;
  float diff_sum;
};

  
class Buttons
{
public: 
  Buttons();
  
  void Update();
  int GetLast(){return last_button_pressed;}
  int GetLast(bool use_default_limiter){if(use_default_limiter){return Limiter() ? GetLast() : 0;}else{return GetLast();}}

  
  int GetHoldingTime(){return last_button_pressed > 0 ? millis() - last_button_pressed_time : 0;}
  unsigned long GetLastPressTime(){return last_button_pressed_time;}

  bool Limiter(int normal_speed_limit=250, int hold_speed_limit=125, int threshold=600);

  //void ForceRelease(){
  //  last_button_pressed = 0;
  //}

  int GetLastAnalogInput(){return last_analog_input;}
  String GetLastButtonName();

  float GetButtonExpectedRead(int button_id);
  float GetButtonInitialExpectedRead(int button_id);
  String GetButtonName(int button_id);
  String GetButtonName(String button_id);
  String GetButtonName(char button_id);
  String GetButtonName(char* button_id);

  void ResetFunctions();
  void SetButtonFunction(int button_id, std::function<void()> func);
private:
  int last_analog_input = 0;
  int last_button_pressed = 0;
  unsigned long last_button_pressed_time = 0;

  unsigned long last_button_passed_limiter_time = 0;
  int last_button_passed_limiter = last_button_passed_limiter;

  float initialExpectedRead[BUTTON_COUNT] = {0};


  Calibrator cali = Calibrator();
  void ResetCalibrator(){cali.button = 0; cali.diff_sum = 0.0; cali.frame_count = 0;}

  int GetButtonIndex(int button_id);
  
};













#endif
