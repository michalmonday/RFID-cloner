
#include "global_includes.h"


int GetNewLineCount(String s){
  int count = 0;
  for(int i = 0; i< s.length(); i++){
    if(s[i] == '\n'){
      count++;
    }
  }
  return count;
}

int GetNewLineCount(char * s){
  int count = 0;
  for(int i = 0; i<strlen(s); i++){
    if(s[i] == '\n'){
      count++;
    }
  }
  return count;
}


String FormatBytes(unsigned long bytes){
  String str = "";
  bytes = float(bytes);
  if(bytes > 100000){
    str = String(bytes / 1000000.0) + " MB";
  }else if(bytes > 100){
    str = String(bytes / 1000.0) + " kB";
  }else{
    str = String(int(bytes)) + " bytes";
  }
  
  return str;
}

