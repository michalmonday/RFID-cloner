#include "NamePicker.h"



void NamePicker::Update(){
  HandleControls();    
  if(nameStatus == CANCELLED){
    //call func
    if(strcmp(file_name, "a\0") != 0){
      strcpy(file_name, "a\0");
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
  strcpy(file_name, "a\0");
  name_picker_x_pos = 0;
}


String NamePicker::GetName(){
  String str(file_name);
  return str;
}

void NamePicker::HandleControls(){
  
  if(int key = buttons.GetLast(true)){ // limiter(normal, hold, threshold)
    int x = name_picker_x_pos;    
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
        char_index++;
        if(char_index >= charset.length()){
          char_index = 0;

        }
        file_name[x] = charset[char_index];    
      }
        break;
      case BUTTON_DOWN:
      {
        char_index--;
        if(char_index < 0){
          char_index = charset.length()-1;
        } 
        file_name[x] = charset[char_index];
      } 
        break;
      case BUTTON_LEFT:
        x--;
        if(x < 0){
          x = 0;
        }
        char_index = charset.indexOf(file_name[x]);
        
        break;
      case BUTTON_RIGHT:
        x++;
        if(x >= MAX_FILENAME){
          x = 0;
        }
        //Serial.println("namePicker::HandleControls - button right - strlen(file_name) = " + String(strlen(file_name)) + "; x = " + String(x));
        if(x >= strlen(file_name)){
          //Serial.println("x > strlen(file_name)");
          char_index = 1;
          file_name[x] = charset[char_index]; 
          file_name[x+1] = '\0'; 
        }else{
          char_index = charset.indexOf(file_name[x]);
        }
        

        break;
      default:
        break;
    }
    name_picker_x_pos = x;
  } 
}
