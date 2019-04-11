#ifndef GUI_H
#define GUI_H

#include <Arduino.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
//#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"
#include "SH1106.h"

extern "C" {
  #include "user_interface.h"
}

#include "Buttons.h"
extern Buttons buttons;
extern char *button_ids;

#include "Notification.h"
extern Notification notification;

#include "MenuOrganizer.h"
extern MenuOrganizer menuOrganizer;

#include "ProgressBar.h"
extern ProgressBar progressBar;

#include "Settings.h"
extern Settings settings;


//#include "global_includes.h"

#include "Time.h"

#define INVALID 255
#define CANCELLED 254

#define MODE_MENU 1
#define MODE_NAME_PICKER 2
#define MODE_NOTIFICATION 3
#define MODE_INTRO 4
#define MODE_BUTTON_CALIBRATION_VIEW 5
#define MODE_PROGRESS_BAR 6
#define MODE_LOCK 7

#define INTRO_LENGTH 1000


//typedef void (*Function)(void);




  
class GUI
{
public: 
  GUI();
  void Init();
  void Update();
  
  void SetMode(int m){gui_mode = m;}

  void SetBrightness(int val);
  void FlipScreenVertically();
private:  
  int gui_mode = 0;
  unsigned long init_time = 0;

  unsigned long intro_begin = 0;
  

  void DrawIntro();
  void DrawMenu();
  void DrawRfidBuffer();
  void DrawNamePicker();
  void DrawNotification();
  void DrawButtonsCalibration();
  void DrawProgressBar();

  
  int DrawScrollIndicator(int items_count, int max_items, int starting_item_index); // return amount of pixels taken horizontally
  void DrawBacklight(int x, int y, int width, int font_size);

  //void DrawCenteredString(char * str, int font_size){DrawCenteredString(String(str), font_size);};
  //void DrawCenteredString(String str, int font_size){DrawCenteredString(str, font_size, 64, 32);}; // +2 works well for font_size 10
  //void DrawCenteredString(String str, int font_size, int x, int y);
  void DrawStringInRect(String txt, int x, int y, int padding, int font_size, OLEDDISPLAY_TEXT_ALIGNMENT alignment);
};



#endif
