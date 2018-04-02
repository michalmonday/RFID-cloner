#ifndef FILES_H
#define FILES_H

#include "FS.h"

#include <Arduino.h>

#define MAX_FILES 200
#define MAX_FILENAME 30

#define MAX_UID 16
struct Card{
  byte UID[MAX_UID]; 
  byte data[64][16];
};



class Files
{
public:
  Files();
  void Init();

  bool Write(char *file_name, Card card);
  bool ReadTo(char *file_name, Card *card);
  void SetTemporaryLastReadFileName(String s);
  String GetTemporaryLastReadFileName(){return temporary_last_read_file_name;}

  void RemoveDir(String directory);
  void DumpDirToSerial(String directory);
  void Format();

  bool Rename(String file_name, String new_name);
  bool Remove(String file_name);
private:
 String temporary_last_read_file_name = "";
  
};























#endif
