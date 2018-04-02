
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
