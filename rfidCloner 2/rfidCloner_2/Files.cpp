#include "Files.h"



Files::Files(){
  
}

void Files::Init(){
  SPIFFS.begin();
  if(SPIFFS.exists("/rfid/.txt")){
    SPIFFS.remove("/rfid/.txt");
  }
}

void Files::Format(){
  SPIFFS.format();
}

void Files::RemoveDir(String directory){
  Dir dir = SPIFFS.openDir(directory);
  while(dir.next()) {
    if(SPIFFS.remove(dir.fileName())){
      Serial.println("Removed file: " + dir.fileName());
    }else{
      Serial.println("Couldn't remove file: " + dir.fileName());
    }
  }
}

void Files::DumpDirToSerial(String directory){
  Dir dir = SPIFFS.openDir(directory);
  while(dir.next()){
    Serial.println("Dir: " + directory + " File name: " + dir.fileName());  
  }
}

bool Files::Rename(String file_name, String new_name){
  String path = "/rfid/" + file_name + ".txt";
  String new_path = "/rfid/" + new_name + ".txt";
  if(SPIFFS.exists(path)){
    if(SPIFFS.rename(path, new_path)){
      return true;
    }else{
      Serial.println("Files::Rename file was found but couldn't be renamed for some reason; path=" + path + "; new_path=" + new_path);
      return false;
    }
  }
  Serial.println("Files::Rename file wasn't found; path=" + path + "; new_path=" + new_path);
  return false;
}

bool Files::Remove(String file_name){
  String path = "/rfid/" + file_name + ".txt";
  if(SPIFFS.exists(path)){
    if(SPIFFS.remove(path)){
      return true;
    }else{
      Serial.println("Files::Remove file was found but couldn't be removed for some reason; path=" + path); 
      return false;
    }
  }else{
   Serial.println("Files::Remove file wasn't found; path=" + path); 
  }
  return false;
}


void Files::SetTemporaryLastReadFileName(String s){
  temporary_last_read_file_name = s;

  String path = "/rfid/" + temporary_last_read_file_name + ".txt";
  if(!SPIFFS.exists(path)){
    File f = SPIFFS.open(path, "w+");
    if (!f) {
      Serial.println("WARNING: Files::SetTemporaryLastReadFileName - temporary_last_read_file could not be created for some reason and wasn't there already. Path:");
      Serial.println(path);
      return;
    }
    //write 'empty' file if it doesn't exist
    for(int i=0; i<MAX_UID; i++){
      uint8_t ones[4] = "11\0";
      f.write(ones, 1);
    }  
    Serial.println("\nBlocks:");
    uint8_t ones[20] = "1111111111111111\0";
    for(int i=0; i<64; i++){
        f.write(ones, 16);
    }
    f.close();
  }
   
}

bool Files::Write(char *file_name, Card card){
  char path[MAX_FILENAME+20];
  sprintf(path, "/rfid/%s.txt\0", file_name);
  File f = SPIFFS.open(path, "w+");
  if (!f) {
      Serial.println("Opening file for writing failed. File path:");
      Serial.println(path);
      return false;
  }

  Serial.println("Writing file:");

  Serial.print("UID - ");
  for(int i=0; i<MAX_UID; i++){
      f.write(card.UID+i, 1);

      char buff[5] = {0};
      sprintf(buff, "%02X ", card.UID[i]);
      Serial.print(buff);
    
  }

  Serial.println("\nBlocks:");
  for(int i=0; i<64; i++){
      f.write(card.data[i], 16);

      for(int j=0; j<16; j++){
        char buff[5] = {0};
        sprintf(buff, "%02X ", card.data[i][j]);
        Serial.print(buff);
      }
      Serial.println();
  }

  f.close();
  return true;
}

bool Files::ReadTo(char *file_name, Card *card){
  char path[MAX_FILENAME+20];
  sprintf(path, "/rfid/%s.txt\0", file_name);
  
  if(SPIFFS.exists(path)){
    File f = SPIFFS.open(path, "r");

    if (!f) {
      Serial.println("Opening file for reading failed. File path:");
      Serial.println(path);
      return false;
    }
    
    //char siz[10];
    //sprintf(siz, "Size of the file: %d", f.size());
    f.seek(0,SeekSet);

    for(int i=0; i<MAX_UID; i++){
      f.readBytes(card->UID+i, 1);
    }

    //Serial.println("\nBlocks:");
    for(int i=0; i<64; i++){
      f.readBytes(card->data[i], 16);
    }
    f.close();
    return true;
  }
  else{
    Serial.println("File path not found. File path:");
    Serial.println(path);
  }
  return false;
}









