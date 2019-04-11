#include "PCinterface.h"



void PCinterface::Update(){
  if(status_action == STATUS_GET_FILE_LIST){
    
  }
}

LinkedList<String> PCinterface::GetFileList(){
  
}

void PCinterface::SendCard(String file_name){
  Card card;
  if(files.ReadTo(file_name, &card)){
    PCinterface::SendCard(card);
  }else{
    Serial.println("PCinterface::SendCard(String file_name) - could not read the card...");
  }
}

/*
#define MAX_UID 16
struct Card{
  byte UID[MAX_UID]; 
  byte data[64][16];
};
*/

void PCinterface::SendCard(Card card){  
  String json = "{\"UID\":\"";

  for(int i=0; i<MAX_UID; i++){
      char buff[3] = {0};
      sprintf(buff, "%02X\0", card.UID[i]);
      json += String(buff);
  }
  json += "\",\"data\":\"";
  
  for(int i=0; i<64; i++){
      for(int j=0; j < 16; j++){
        char buff[3] = {0};
        sprintf(buff, "%02X\0", card.data[i][j]);
        json += String(buff);
      }
  } 
  json += "\"}\r\n";
  
  Serial.print(json);
}



