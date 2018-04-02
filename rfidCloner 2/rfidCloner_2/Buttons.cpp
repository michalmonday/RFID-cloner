#include "Buttons.h"

#define THRESHOLD_INPUT 50                 // if input > THRESHOLD then key was pressed
#define INPUT_TOLERANCE 10                 //so if 110 or 90 will be received when BUTTON_UP is pressed then it will be accepted anyway
#define BUTTON_COUNT 6

int buttonAnalogInputMap[BUTTON_COUNT][2] = {
 100, BUTTON_UP,
 191, BUTTON_DOWN,
 315, BUTTON_LEFT,
 449, BUTTON_RIGHT,
 629, BUTTON_YES,
 875, BUTTON_NO,
};

void Buttons::Update(){   
  int last_analog_input = analogRead(A0);
  if(last_analog_input > THRESHOLD_INPUT){
    if(!last_button_pressed){
      last_button_pressed_time = millis();
    }
    
    for(int i = 0; i < BUTTON_COUNT; i++){
      int lower_border = buttonAnalogInputMap[i][0] - INPUT_TOLERANCE;
      int higher_border = buttonAnalogInputMap[i][0] + INPUT_TOLERANCE;
      if(last_analog_input >= lower_border && last_analog_input <= higher_border){
        last_button_pressed = buttonAnalogInputMap[i][1];
      }
    }
  }
  else{
    last_button_pressed = 0;
  }
}

bool Buttons::Limiter(int normal_speed_limit, int hold_speed_limit, int threshold){  
  int limit = (GetHoldingTime() > threshold) ? hold_speed_limit : normal_speed_limit;
  bool ret = ((millis() - last_button_passed_limiter_time) > limit) || (last_button_passed_limiter != last_button_pressed);

  if(ret == true){
    //Serial.println("Buttons::Limiter = true; holding time = " + String(GetHoldingTime()) + "; time since last button passed limiter = " + String(millis() - last_button_passed_limiter_time));
    last_button_passed_limiter_time = millis();
    last_button_passed_limiter = last_button_pressed;
    //if(last_button_pressed > 0){
      //Serial.println("Buttons::Limiter = true && last_button_pressed > 0; time = " + String(last_button_passed_limiter_time));
    //}
  }
  
  return ret;
}









