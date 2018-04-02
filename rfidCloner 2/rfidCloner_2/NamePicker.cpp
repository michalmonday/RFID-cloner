#include "NamePicker.h"



void NamePicker::Update(){
  HandleControls();    
  if(nameStatus == CANCELLED){
    //call func
    if(strcmp(file_name, "a             \0") != 0){
      strcpy(file_name, "a             \0");
      nameStatus = NOT_READY;
      name_picker_x_pos = 0;
    }else{
      onCancelFunc();
      Reset();
    }
  }else if(nameStatus != NOT_READY){
    // do something with the name (?) or something
    //call func
    onSuccessFunc();
    Reset();
  }
}

void NamePicker::Reset(){
  nameStatus = NOT_READY;
  strcpy(file_name, "a             \0");
  name_picker_x_pos = 0;
}


String NamePicker::GetName(){
  String str(file_name);
  return str;
}

void NamePicker::HandleControls(){
  int x = name_picker_x_pos;
 
    
  //sprintf(buff, ">%s", file_name);
  //display.drawString(64, 32-8, buff);
  
  //delay((millis() - lastPressTime > 600 && !debounced) ? 90 : 250); // speed up on hold

  
  if(int key = buttons.GetLast(true)){ // limiter(normal, hold, threshold)
    //char buff3[100]={0};
    //sprintf(buff3, "\nGUI::HandleGettingName:\nmillis() = %d\nlastPressedTime = %d\n!debounced = %d\n\0", millis(), lastPressTime, !debounced);
    //Serial.println(buff3);
    
      switch(key){
      case BUTTON_YES:
      //format the name properly (remove empty spaces)
        for(int i=0; i< MAX_FILENAME; i++){
          if(file_name[i] == ' '){
            if(i < MAX_FILENAME-1){
              if(file_name[i+1] != 32 && file_name[i+1]){
                file_name[i] = '_';
              }else{
                file_name[i] = '\0'; 
                nameStatus = READY;
              }
            }else{
              file_name[i] = '\0'; 
              nameStatus = CANCELLED;
            }
          }
        }
        file_name[MAX_FILENAME-1] = '\0';

        
        nameStatus = READY;
        break;
      case BUTTON_NO:
        nameStatus = CANCELLED;
        break;
      case BUTTON_UP:
      {
        if(file_name[x] == ' '){
          file_name[x] = 'a';
        }
        else if(file_name[x] == 'z'){
          file_name[x] = ' ';
        }
        else{
          file_name[x]++;
        }      
      }
        break;
      case BUTTON_DOWN:
      {
        if(file_name[x] == ' '){
          file_name[x] = 'z';
        } 
        else if(file_name[x] == 'a'){
          file_name[x] = ' ';
        }
        else{
          file_name[x] -= 1;
        }
      } 
        break;
      case BUTTON_LEFT:
        x--;
        if(x < 0){
          x = 0;
        }
        break;
      case BUTTON_RIGHT:
        x++;
        if(x >= MAX_FILENAME){
          x = 0;
        }
        break;
      default:
        break;
    }
  } 
  name_picker_x_pos = x;
}
