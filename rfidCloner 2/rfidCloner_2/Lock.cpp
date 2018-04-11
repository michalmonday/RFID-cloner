#include "Lock.h"

#include "Buttons.h"
extern Buttons buttons;

void Lock::Update(){
  if(int key = buttons.GetLast(true)){
    switch(key){
      case BUTTON_YES:
        if(TryCombo() == true){
          solved = true;
        }else{
          solved = false;
        }
        break;
      case BUTTON_NO:
        try_sequence = "";
        break;
      default:
        try_sequence += String(key);
        break;  
    }
  }
}



