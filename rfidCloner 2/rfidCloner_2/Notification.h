#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <Arduino.h>

#include "Buttons.h"
extern Buttons buttons;

#include <functional>

#include "global_includes.h"

#define MAX_NOTIFICATION_LEVELS 5

#define NOTIFICATION_TIME 500

struct NotificationLevel{
  String msg = "";
  XY pos = {0,0};
  unsigned long started = 0;
  int timeout = 0;
  bool active = false;
  bool useConditionalFunc = false;
  std::function<bool()> conditionalFunction = [](){return true;};
  std::function<void()> onAcceptFunction = [](){};
  std::function<void()> onDeclineFunction = [](){};
};


class Notification
{
public:
  void Init();
  void Update();
  void Reset(int i);

  void Set(int id_level, String message, XY pos);
  void Set(int id_level, String message, XY pos, int timeout_=0);

  bool IsAvailable(){return GetHighestLevel() > -1;}
  String GetMsg(){return level[GetHighestLevel()].msg;}
  XY GetPos(){return level[GetHighestLevel()].pos;}
  unsigned long GetLifeSpan(int i){return millis() - level[i].started;}
  
  void SetConditionalFunction(int i, std::function<bool()> func){level[i].useConditionalFunc = true; level[i].conditionalFunction=func;}// function that returns true if certain time passed or if any button was pressed
  void OnAccept(int i, std::function<void()> func){level[i].onAcceptFunction=func;}
  void OnDecline(int i, std::function<void()> func){level[i].onDeclineFunction=func;}

private:
  NotificationLevel level[MAX_NOTIFICATION_LEVELS];
  int GetHighestLevel();

  void MembersDump(String src);
};























#endif
