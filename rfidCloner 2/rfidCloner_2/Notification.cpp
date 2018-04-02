#include "Notification.h"


void Notification::Init(){
  for(int i = MAX_NOTIFICATION_LEVELS-1; i >= 0; i--){
    Reset(i);
  }
}

int Notification::GetHighestLevel(){
  for(int i = MAX_NOTIFICATION_LEVELS-1; i >= 0; i--){
    if(level[i].active == true){
      return i;
    }
  }
  Serial.println("Notification::GetHighestLevel() = -1"); delay(500);
  return -1;
}

void Notification::Update(){

  int i = GetHighestLevel();
  //MembersDump("Notification::Update");
  if(i > -1){
    if(level[i].useConditionalFunc || (level[i].timeout > 0 && ((millis() - level[i].started) > level[i].timeout))){
      if(level[i].conditionalFunction()){
        Serial.println("Notification::Update - conditional function returned true.");
        level[i].onAcceptFunction();
        Reset(i);
        return;
      }
    }
    
    buttons.Update();
    if(int key = buttons.GetLast(true)){
      if(key == BUTTON_YES){
        level[i].onAcceptFunction();
        Reset(i);
      }else if(key == BUTTON_NO){
        level[i].onDeclineFunction();
        Reset(i);  
      }  
    }
  }
}

void Notification::Reset(int i){
  level[i].useConditionalFunc = false;
  level[i].conditionalFunction = [](){return true;};
  level[i].onAcceptFunction = [](){};
  level[i].onDeclineFunction = [](){};
  level[i].started = 0;
  level[i].timeout = 0;
  level[i].msg = "";
  level[i].active = false;
}


void Notification::Set(int i, String message, XY pos_){
  level[i].msg = message; 
  level[i].pos = pos_; 
  level[i].started = millis(); 
  level[i].timeout = 0;
  level[i].useConditionalFunc = false; 
  level[i].active = true;

  //MembersDump("Notification::Set");
}

void Notification::Set(int i, String message, XY pos_, int timeout_){
  level[i].msg = message; 
  level[i].pos = pos_; 
  level[i].started = millis(); 
  level[i].timeout = timeout_; 
  level[i].useConditionalFunc = false;
  level[i].active = true;

  //MembersDump("Notification::Set");
}




void Notification::MembersDump(String src){
  char i = 48;
  Serial.print("Notification::MembersDump var-"); Serial.println(i++);
  Serial.println(GetHighestLevel());
  Serial.print("Notification::MembersDump var-"); Serial.println(i++);
  Serial.println(level[GetHighestLevel()].msg);
  Serial.print("Notification::MembersDump var-"); Serial.println(i++);
  Serial.println(level[GetHighestLevel()].pos.x);
  Serial.print("Notification::MembersDump var-"); Serial.println(i++);
  Serial.println(level[GetHighestLevel()].pos.y);
  Serial.print("Notification::MembersDump var-"); Serial.println(i++);
  Serial.println(level[GetHighestLevel()].started);
  Serial.print("Notification::MembersDump var-"); Serial.println(i++);
  Serial.println(level[GetHighestLevel()].timeout);
  Serial.print("Notification::MembersDump var-"); Serial.println(i++);
  Serial.println(level[GetHighestLevel()].useConditionalFunc);
  Serial.print("Notification::MembersDump var-"); Serial.println(i++);
  Serial.println(level[GetHighestLevel()].active);

  Serial.println("Notification::MembersDump var-end");

  i = GetHighestLevel();
  char buff[300]={0};
  sprintf(buff, " members dump:\nGetHighestLevel() = %d\nlevel[i].msg = %s\nlevel[i].pos.x = %d\nlevel[i].pos.y = %d\nlevel[i].started = %d\nlevel[i].timeout = %d\nlevel[i].useConditionalFunc = %d\nlevel[i].active = %d\n", 
  i,
  (char*)level[i].msg.c_str(),
  level[i].pos.x,
  level[i].pos.y,
  level[i].started,
  level[i].timeout,
  level[i].useConditionalFunc,
  level[i].active
  );
  String s(buff);
  src.concat(s);
  Serial.println();
  Serial.println(s);
}






