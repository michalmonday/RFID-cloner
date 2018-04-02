#include "Time.h"
// source: http://forum.arduino.cc/index.php?topic=42211.0

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

String GetTimeSince(long milliseconds){
  long val = milliseconds / 1000;
  //int days = elapsedDays(val);
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  //int seconds = numberOfSeconds(val);

  String ret = "";
  if(hours < 10){
    ret += '0';
  }
  ret += String(hours) + ':';
  if(minutes < 10){
    ret += '0';
  }  
  ret += String(minutes);
  return ret;
}







