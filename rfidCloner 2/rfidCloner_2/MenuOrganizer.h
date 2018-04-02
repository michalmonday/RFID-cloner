#ifndef MENUORGANIZER_H
#define MENUORGANIZER_H

#include <Arduino.h>


#define MAX_MENU 20
#define MAX_MENU_TEXT 40

#define INVALID 255
#define CANCELLED 254


#include "LinkedList.h"
#include "Buttons.h"
extern Buttons buttons;

#include "Rfid.h"
extern Rfid rfid;






#include "NamePicker.h"
extern NamePicker namePicker;

#include "Notification.h"
extern Notification notification;


struct MenuOption{
  char text[MAX_MENU_TEXT];
  bool active;
  std::function<void()> function;
};


struct Menu{
  Menu *parent;
  int len = 0;
  int active_len = 0;
  XY current_option = {0,0};
  int selected = INVALID;
  int max_options_displayed_default = 5;
  int max_options_displayed = max_options_displayed_default;
  int starting_option = 0;
  int current_option_relative = 0;
  LinkedList<MenuOption> *options = new LinkedList<MenuOption>;
  LinkedList<MenuOption> *horizontal_options = new LinkedList<MenuOption>;
};



class MenuOrganizer
{
public:
  void Init();

  void Update();
  void ResetMenuState(Menu *_menu);

  LinkedList<MenuOption> GetActiveOptions();
  LinkedList<String> GetOnScreenOptionNames();
  int GetCurrentOnScreenOption();
  
  LinkedList<MenuOption> GetOnScreenOptions();
  void MenuDataDump(String src);

  bool HasHorizontalOptions(){return HasHorizontalOptions(menu);}
  bool HasHorizontalOptions(Menu *_menu){return (bool)menu->horizontal_options->size();}
  
  String GetHorizontalOptionName(){return String(menu->horizontal_options->get(menu->current_option.x).text);}
  
private:
  Menu *menu;
  Menu *mainMenu = new Menu; 
  Menu *dirMenu = new Menu; 

  Menu *manageFilesMenu = new Menu; 

  
  void SetMenu(Menu *menu);
  void AddOption(Menu *_menu, MenuOption option);
  void AddHorizontalOption(Menu *_menu, MenuOption option);
  void ClearOptions(Menu *_menu);
  void SetOptionName(Menu * _menu, int opt_num, String opt_name){MenuOption mo = _menu->options->get(opt_num); strcpy(mo.text, (char*)opt_name.c_str()); _menu->options->set(opt_num, mo);}
  
  void HandleControls();
  bool Cancelled();
  bool WasSelected();
  int GetSelection();
  void ResetSelection();
  void ActivateOption(Menu *_menu, int num);
  void DeactivateOption(Menu *_menu, int num);
  void UpdateActiveMenu();
  void ShiftCurrentOption(int buttonID);
  void Notify(String msg, int timeout);
  void Notify(String msg, std::function<void()> onAccept, std::function<void()> onDecline);
  void Notify(int level, String msg, int timeout);
  void Notify(int level, String msg, std::function<void()> onAccept, std::function<void()> onDecline);
  void Notify(bool res);
};




#endif



