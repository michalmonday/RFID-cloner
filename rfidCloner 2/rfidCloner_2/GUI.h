#ifndef GUI_H
#define GUI_H

#include <Arduino.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"

extern "C" {
  #include "user_interface.h"
}

#include "Buttons.h"
extern Buttons buttons;

#include "Notification.h"
extern Notification notification;

#include "MenuOrganizer.h"

extern MenuOrganizer menuOrganizer;


#include "global_includes.h"
#include "Time.h"

#define INVALID 255
#define CANCELLED 254

#define MODE_MENU 1
#define MODE_NAME_PICKER 2
#define MODE_NOTIFICATION 3


//typedef void (*Function)(void);




  
class GUI
{
public: 
  GUI();
  void Init();
  void Update();
  
  void SetMode(int m){gui_mode = m;}

private:

  //Menu active_menu;
  
  //int menu_len;
  //int active_menu_len;
  //XY current_option = {0,0};
  //int selected = INVALID;
  //int max_options_displayed_default = 5;
  //int max_options_displayed = max_options_displayed_default;
  //int starting_option = 0;
  //int current_option_relative = 0;
  
  int gui_mode = MODE_MENU;

  void DrawMenu();
  void DrawRfidBuffer();
  void DrawNamePicker();
  void DrawNotification();

  //void DrawCenteredString(char * str, int font_size){DrawCenteredString(String(str), font_size);};
  //void DrawCenteredString(String str, int font_size){DrawCenteredString(str, font_size, 64, 32);}; // +2 works well for font_size 10
  //void DrawCenteredString(String str, int font_size, int x, int y);

  void DrawStringInRect(String txt, int x, int y, int padding, int font_size, OLEDDISPLAY_TEXT_ALIGNMENT alignment);
};



#endif
