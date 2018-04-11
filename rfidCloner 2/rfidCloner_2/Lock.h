#ifndef LOCK_H
#define LOCK_H

#include <Arduino.h>

class Lock
{
public:
  void Update();

  void SetState(bool state){use_lock = state;}
  bool GetState(){return use_lock;}
  void SetCorrectSequence(String s){correct_sequence = s;}
  String GetCorrectSequence(){return correct_sequence;}
  int GetTryLength(){return try_sequence.length();}
  bool Solved(){return solved;}
  
  //void SetWrongTriesLimit(int i);
private:
  String correct_sequence = "";
  String try_sequence = "";
  bool use_lock = false;
  //int wrong_tries = 0;
  bool solved = false;

  bool TryCombo(){return try_sequence == correct_sequence;}
};























#endif
