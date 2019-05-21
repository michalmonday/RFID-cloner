#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <MFRC522.h>

#include "Files.h"
extern Files files;

#define NOT_READY 255
#define CANCELLED 254
#define READY 253

#define REQUESTED_READ 1
#define REQUESTED_WRITE 2

#define READ_SOURCE_CARD 1
#define READ_SOURCE_FILE 2


class Rfid
{
public:
  void Init();

  void RequestWriteCard();
  void RequestReadCard();
  void ResetRequest(){request_status = 0;}
  
  bool SaveToFile(char *file_name);
  bool SaveToFile(String s){return SaveToFile((char *)s.c_str());}
  
  bool ReadFromFile(char *file_name);
  void DumpToSerial();
  
  void Update();

  bool WasCardPresented(){return was_card_presented;}

  void OnCardPresented(std::function<void()> func){onCardPresentedFunc=func;}
  void OnSuccess(std::function<void()> func){onSuccessFunc=func;}
  void OnFail(std::function<void()> func){onFailFunc=func;}

  unsigned long GetLastReadTime(){return last_read_time;}
  String GetLastReadFileName(){return last_read_file_name;}
  String GetLastReadSourceType();

  byte * GetUID();
private:
  Card card;
  int read_status = NOT_READY;
  int request_status = 0;
  
  unsigned long last_read_time = 0;
  int last_read_source = 0; // READ_SOURCE_CARD READ_SOURCE_FILE
  String last_read_file_name = "";
  
  bool was_card_presented = false;
  
  bool Try_key(MFRC522::MIFARE_Key *key);
  bool IsCardAvailable(); 

  bool WriteCard();
  bool ReadCard();

  std::function<void()> onSuccessFunc = [](){};
  std::function<void()> onCardPresentedFunc = [](){};
  std::function<void()> onFailFunc = [](){};
};























#endif
