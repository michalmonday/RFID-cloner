#include "Settings.h"

#define MAX_SETTING_LEN 50

// Brightness

bool Settings::Set(String key, String val){
  String path = "/settings/" + key;

  File f = SPIFFS.open(path, "w+");
  if (f) {
    for(int i = 0; i < val.length(); i++){
      f.write(val[i]);
    }
    return true;
  }else{
    Serial.println("Settings::Set - Opening file for writing failed. File path:" + path);
    return false;
  }
}




String Settings::Get(String key, String backup_value){
  String path = "/settings/" + key;
  if(SPIFFS.exists(path)){
    File f = SPIFFS.open(path, "r");
    if (f) {
      f.seek(0,SeekSet);
      char buff[MAX_SETTING_LEN] = {0};
      f.readBytes(buff, f.size());
      buff[f.size()] = 0;
      return String(buff);      
    }else{
      Serial.println("Opening file for writing failed. File path: " + path);
      return backup_value;
    }
  }else{
    Serial.println("Settings::Get - File doesn't exist: " + path);
    Serial.println("Creating that file...");
    Set(key, backup_value);
    return backup_value;
  }
}





