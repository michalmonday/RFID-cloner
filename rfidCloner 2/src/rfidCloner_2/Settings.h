#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

#include "Files.h"
extern Files files;



class Settings
{
public:
  void Load();
  
  bool Set(String key, String val);
  bool Set(String key, int val){Set(key, String(val));}
  bool Set(int key, int val){Set(String(key), String(val));}
  bool Set(int key, String val){Set(String(key), val);}

  String Get(String key, String backup_value);
  String Get(String key, int backup_value){return Get(key, String(backup_value));}
  String Get(String key){return Get(key, "0");}
  
private:

};























#endif
