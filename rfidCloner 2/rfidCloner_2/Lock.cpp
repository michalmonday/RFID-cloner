#include "Lock.h"

#include "Buttons.h"
extern Buttons buttons;

void Lock::Update(){
  if(int key = buttons.GetLast(true)){
    switch(key){
      case BUTTON_YES:
        onReady();
        input_sequence = "";
        break;
      case BUTTON_NO:
        if(input_sequence.length() > 0){
          input_sequence = "";
        }else{
          onCancel();
        }
        break;
      default:
        input_sequence += String(key);
        break;  
    }
  }
}

void Lock::Reset(){
  input_sequence = "";
  onReady = [](){};
  onCancel = [](){};
}


String Lock::GetFormattedCorrectSequence(String delimeter){
  String ret = "";
  if(correct_sequence.length() > 0){
    for(int i=0; i<correct_sequence.length(); i++){
      if(i>0){ret += delimeter;}
      ret += buttons.GetButtonName(correct_sequence[i]);
    }
  }else{
    ret = "Empty";
  }
  return ret;
}



