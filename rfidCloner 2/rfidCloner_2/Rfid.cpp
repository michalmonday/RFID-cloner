/*
  All the RFID functionality in the code below is copy/pasted from this RFID library:
  https://github.com/miguelbalboa/rfid/

  I just reshaped it to use it easily with this project.
*/

#include "Rfid.h"

#include <SPI.h>
#include <MFRC522Hack.h>
constexpr uint8_t RST_PIN = D3;         
constexpr uint8_t SS_PIN = D8;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522Hack mfrc522Hack(&mfrc522);  // Create MFRC522Hack instance.

MFRC522::StatusCode status;
MFRC522::MIFARE_Key key;

byte buffer[18];

#define NR_KNOWN_KEYS   8
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};



void Rfid::Init(){
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  //mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details


}


String Rfid::GetLastReadSourceType(){
  String src = "";
  if(last_read_source > 0){
    if(last_read_source == READ_SOURCE_CARD){
      src = "Card";
    }else if(last_read_source == READ_SOURCE_FILE){
      src = GetLastReadFileName() + " file";
    }
  }else{
    src = "Empty";
  }
  return src;
}



void Rfid::Update(){
  if(request_status == REQUESTED_READ){
    if(IsCardAvailable()){
      was_card_presented = true;
      onCardPresentedFunc();
      was_card_presented = false;
      if(ReadCard()){
        onSuccessFunc();
        last_read_time = millis();
        last_read_source = READ_SOURCE_CARD;
        request_status = 0;
        
        SaveToFile(files.GetTemporaryLastReadFileName());
      }else{
        onFailFunc();
      }
    }
  }else if(request_status == REQUESTED_WRITE){
    if(IsCardAvailable()){
      was_card_presented = true;
      onCardPresentedFunc();
      was_card_presented = false;
      if(WriteCard()){
        onSuccessFunc();
        request_status = 0;
      }else{
        onFailFunc();
      }
    }
  }
}


void Rfid::RequestWriteCard(){
  request_status = REQUESTED_WRITE;
}
void Rfid::RequestReadCard(){
  request_status = REQUESTED_READ;
}


bool Rfid::IsCardAvailable(){
  return (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial());
}

bool Rfid::Try_key(MFRC522::MIFARE_Key *key){
    boolean result = false;
    
    for(byte block = 0; block < 64; block++){       
      // Serial.println(F("Authenticating using key A..."));
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          ////Serial.print(F("PCD_Authenticate() failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
          return false;
      }
  
      // Read block
      byte byteCount = sizeof(buffer);
      status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
      if (status != MFRC522::STATUS_OK) {
          //Serial.print(F("MIFARE_Read() failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
      }
      else {
          // Successful read
          result = true;
          //Serial.print(F("Success with key:"));
          //dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
          //Serial.println();
          
          // Dump block data
          //Serial.print(F("Block ")); Serial.print(block); Serial.print(F(":"));
          //dump_byte_array1(buffer, 16); //omzetten van hex naar ASCI
          ////Serial.println();
          
          for (int p = 0; p < 16; p++) //De 16 bits uit de block uitlezen
          {
            card.data[block][p] = buffer[p];
            //Serial.print(waarde[block][p]);
            //Serial.print(" ");
          }
       }
    }

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
}

bool Rfid::ReadCard(){
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  //Serial.println(mfrc522.PICC_GetTypeName(piccType));

  memset(card.UID, 0, MAX_UID);
  
  for(byte i=0; i<mfrc522.uid.size; i++)
  {
    card.UID[i] = mfrc522.uid.uidByte[i];
  }
  card.UID[mfrc522.uid.size] = 0 ;
  
  // Try the known default keys
  MFRC522::MIFARE_Key key;
  for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
      // Copy the known key into the MIFARE_Key structure
      for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
          key.keyByte[i] = knownKeys[k][i];
      }
      // Try the key
      if (Try_key(&key)) {
          // Found and reported on the key and block,
          // no need to try other keys for this PICC
          return true;
      }
  }
  return false;
}

bool Rfid::WriteCard(){
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

  for(int i = 4; i <= 62; i++){ //De blocken 4 tot 62 kopieren, behalve al deze onderstaande blocken (omdat deze de authenticatie blokken zijn)
    if(i == 7 || i == 11 || i == 15 || i == 19 || i == 23 || i == 27 || i == 31 || i == 35 || i == 39 || i == 43 || i == 47 || i == 51 || i == 55 || i == 59){
      i++;
    }
 
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, i, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        //Serial.print(F("PCD_Authenticate() failed: "));
        //Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }
      
    // Authenticate using key B
    //Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, i, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        //Serial.print(F("PCD_Authenticate() failed: "));
        //Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }
    
    // Write data to the block
    //Serial.print(F("Writing data into block ")); 
    //Serial.print(block);
    //Serial.println("\n");

     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(i, card.data[i], 16);
      if (status != MFRC522::STATUS_OK) {
        //Serial.print(F("MIFARE_Write() failed: "));
        //Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
      }
  }

  if ( mfrc522Hack.MIFARE_SetUid(card.UID, (byte)4, true) ) {
    //Serial.println(F("Wrote new UID to card."));
  }
  else{
    return false;
  }
  
  mfrc522.PICC_HaltA();       // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}


bool Rfid::SaveToFile(char *file_name){
  return files.Write(file_name, card);
}

bool Rfid::ReadFromFile(char *file_name){
  if(files.ReadTo(file_name, &card)){
    last_read_time = millis();
    last_read_source = READ_SOURCE_FILE;
    last_read_file_name = String(file_name);
    return true;
  }
  return false;
}

void Rfid::DumpToSerial(){
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));// Dump debug info about the card; PICC_HaltA() is automatically called
  Serial.print("UID - ");
  for(int i=0; i<MAX_UID; i++){
    char buff[5] = {0}; 
    sprintf(buff, "%02X ", card.UID[i]);
    Serial.print(buff);
  }

  Serial.println("\nBlocks:");
  for(int i=0; i<64; i++){
    for(int j=0; j<16; j++){
      char buff[5] = {0};
      sprintf(buff, "%02X ", card.data[i][j]);
      Serial.print(buff);
    }
    Serial.println();
  }
}











