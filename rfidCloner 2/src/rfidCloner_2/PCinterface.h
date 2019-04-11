#ifndef PCINTERFACE_H
#define PCINTERFACE_H

#include <Arduino.h>
#include <LinkedList.h>
#include "Rfid.h"


#define STATUS_GET_FILE_LIST 1

class PCinterface
{
public:
  void Update();
  LinkedList<String> GetFileList();
  void SendCard(char *file_name){SendCard(String(file_name));}
  void SendCard(String file_name);
  void SendCard(Card c);
private:
  int status_action = 0;
};























#endif
