#include "ProgressBar.h"


void ProgressBar::Update(){
  int key = buttons.GetLast(true);
  if(key){
    if(key == BUTTON_YES){
      onSuccessFunc();
      Reset();
    }else if(key == BUTTON_NO){
      onFailFunc();
      Reset();
    }
  }
  current_value = getValFunc(key);
  if(current_value > high_border){current_value = low_border;}
  if(current_value < low_border){current_value = high_border;}
}

void ProgressBar::Reset(){
  label = "";
  current_value = 0;
  high_border = 100;
  low_border = 0;
  onSuccessFunc = [](){};
  onFailFunc = [](){};
  getValFunc = [](int i){return 0;};
}

