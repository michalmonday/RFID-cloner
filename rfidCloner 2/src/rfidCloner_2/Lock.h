#ifndef LOCK_H
#define LOCK_H

#include <Arduino.h>

class Lock
{
public:
  void Update();
  void Reset();

  void SetState(bool state){use_lock = state;}
  bool GetState(){return use_lock;}
  void SetCorrectSequence(String s){correct_sequence = s;}
  String GetCorrectSequence(){return correct_sequence;}
  String GetFormattedCorrectSequence(String delimeter);
  int GetTryLength(){return input_sequence.length();}
  String GetInput(){return input_sequence;}
  bool TrialMatchesCorrectSequence(){return correct_sequence == input_sequence;}
  void OnReady(std::function<void()> func){onReady = func;}
  void OnCancel(std::function<void()> func){onCancel = func;}
  
  //void SetWrongTriesLimit(int i);
private:
  String correct_sequence = "";
  String input_sequence = "";
  bool use_lock = false;
  //int wrong_tries = 0;

  std::function<void()> onReady = [](){};
  std::function<void()> onCancel = [](){};
};























#endif
