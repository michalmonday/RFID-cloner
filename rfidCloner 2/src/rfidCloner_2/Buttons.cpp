#include "Buttons.h"


#define THRESHOLD_INPUT 20.0                 // if input > THRESHOLD then key was pressed
#define INPUT_TOLERANCE 0.12                 //so if 112 or 88 will be received when BUTTON_UP is pressed then it will be accepted anyway

/*
    It appears that if a battery power source is used instead of using PC USB connection then the buttons become less reliable.
    Battery voltage gets gradually lower. Because of that the analog reading of buttons will decrease. That's why automatical calibration 
    will take place when buttons are pressed. Potentially it could lead to situations where it auto-calibrates incorrectly and wrong or no buttons 
    get pressed when they actually are pressed. If that will be the case then the code will have to be adjusted.
*/


struct ButtonMap{
  float expected_read;
  int button_id;
  String name_txt;
  std::function<void()> onRelease;
};



ButtonMap buttonMap[BUTTON_COUNT] = {
 50.5, BUTTON_NONE, "NONE", [](){},
 100.0, BUTTON_UP, "UP", [](){},
 191.0, BUTTON_DOWN, "DOWN", [](){},
 315.0, BUTTON_LEFT, "LEFT", [](){},
 449.0, BUTTON_RIGHT, "RIGHT", [](){},
 629.0, BUTTON_YES, "ACCEPT", [](){},
 871.0, BUTTON_NO, "DECLINE", [](){},
};

void Buttons::Init(){
  for(int i = 0; i< BUTTON_COUNT; i++){
    initialExpectedRead[i] = int(buttonMap[i].expected_read);
  }
}

void Buttons::Update(){   
  last_analog_input = analogRead(A0);
  
  if(last_analog_input > THRESHOLD_INPUT){
    if(!last_button_pressed){
      last_button_pressed_time = millis();
    }
    
    for(int i = 0; i < BUTTON_COUNT; i++){
      float lower_border = buttonMap[i].expected_read - buttonMap[i].expected_read * INPUT_TOLERANCE;
      float higher_border = buttonMap[i].expected_read + buttonMap[i].expected_read * INPUT_TOLERANCE;
      if(last_analog_input >= lower_border && last_analog_input <= higher_border){
        last_button_pressed = buttonMap[i].button_id;

        // get difference between expected reading and automatically calibrate all the expected buttons input values
        //float diff = last_analog_input - buttonMap[i].expected_read;
        //float percent_diff = diff / buttonMap[i].expected_read;
        

        if(last_button_pressed != cali.button){
          ResetCalibrator();
          cali.button = last_button_pressed;
        }
      
        cali.diff_sum += (float(last_analog_input) - buttonMap[i].expected_read) / buttonMap[i].expected_read;
        cali.frame_count++;
      
        if(cali.frame_count > 5){
          /*
              Use average difference from few frames for calculation to prevent bug due to occasional spikes
           */
          for(int j = 0; j < BUTTON_COUNT; j++){
             buttonMap[j].expected_read += buttonMap[j].expected_read * (cali.diff_sum / float(cali.frame_count));
          }   
          ResetCalibrator();
        }     
      }
    }
  }
  else{
    if(last_button_pressed){
      buttonMap[GetButtonIndex(last_button_pressed)].onRelease();
    }
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

int Buttons::GetButtonIndex(int button_id){
  for(int i = 0; i < BUTTON_COUNT; i++){
    if(buttonMap[i].button_id == button_id){
      return i;
    }
  }
  return 0;
}

float Buttons::GetButtonExpectedRead(int button_id){
  return  buttonMap[GetButtonIndex(button_id)].expected_read;
}

int Buttons::GetButtonInitialExpectedRead(int button_id){
  return initialExpectedRead[GetButtonIndex(button_id)];
}

String Buttons::GetButtonName(int button_id){
  return buttonMap[GetButtonIndex(button_id)].name_txt;
}

String Buttons::GetButtonName(String button_id){
  char buff[2] = {button_id[0], 0};
  return GetButtonName((int)strtol(buff, NULL, 10));
}

String Buttons::GetButtonName(char button_id){
  return GetButtonName((int)button_id);
}

String Buttons::GetLastButtonName(){
  if(last_button_pressed){
    return GetButtonName(last_button_pressed);
  }
  return "-";
}

void Buttons::ResetFunctions(){
  for(int i = 0; i< BUTTON_COUNT; i++){
    buttonMap[i].onRelease = [](){};
  }
}

/*
void Buttons::SetButtonFunction(int button_id, std::function<void()> func){
  for(int i = 0; i < BUTTON_COUNT; i++){
    if(buttonMap[i].button_id == button_id){
      buttonMap[button_id].onRelease = func;
      return;
    }
  }

  Serial.println("Buttons::GetButtonName - NONE FOUND - button_id=" + String(button_id));
}
*/











