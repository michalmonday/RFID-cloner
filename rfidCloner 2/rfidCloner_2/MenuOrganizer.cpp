#include "MenuOrganizer.h"

 
#include "GUI.h"
extern GUI gui;

void MenuOrganizer::Update(){
  HandleControls();
  if(menu->selected != INVALID){   
    if(menu->selected != CANCELLED){
      MenuOption mo = GetActiveOptions().get(menu->selected);
      Serial.print("Picked menu option - "); Serial.println(mo.text);
      mo.function();
    }else{
      // switch to some parent menu or something
      ResetMenuState(menu);
      SetMenu(menu->parent);
      //clean up any possible runtime actions regardless of the option cancelled
      //rfid.ResetRequest(); // idk
    }
    menu->selected = INVALID;
  }
}

void MenuOrganizer::ResetMenuState(Menu *_menu){
  _menu->current_option = {0,0};
  _menu->selected = INVALID;
  _menu->starting_option = 0;
  _menu->current_option_relative = 0;
}

LinkedList<String> MenuOrganizer::GetOnScreenOptionNames(){
  LinkedList<MenuOption> screen_options = GetOnScreenOptions(); 
  LinkedList<String> names;
  for(int i=0; i < screen_options.size(); i++){ 
    String str(screen_options.get(i).text);
    names.add(str);
  }
  return names;
}

LinkedList<MenuOption> MenuOrganizer::GetOnScreenOptions(){
  LinkedList<MenuOption> screen_options;
  LinkedList<MenuOption> active_options = GetActiveOptions();
  for(int i = menu->starting_option; i < menu->starting_option + menu->max_options_displayed && i < menu->starting_option + active_options.size(); i++){
    screen_options.add(active_options.get(i));
  }
  return screen_options;
}

LinkedList<MenuOption> MenuOrganizer::GetActiveOptions(){
  LinkedList<MenuOption> opts;
  for(int i=0; i < menu->len; i++){
    if(menu->options->get(i).active){  
      opts.add(menu->options->get(i));
    }
  }
  return opts;
}


/*
LinkedList<std::function<void()>> MenuOrganizer::GetActiveOptionFunctions(){
  LinkedList<std::function<void()>> funcs;
  for(int i=0; i < menu->len; i++){
    if(menu->options->get(i).active){
      funcs.add(menu->options->get(i).function);
    }
  }
  return funcs;
}
*/

int MenuOrganizer::GetCurrentOnScreenOption(){
  return menu->current_option_relative;
}

void MenuOrganizer::Notify(String msg, int timeout){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(0, msg, {64, 32-5}, timeout);   
  notification.OnAccept(0, [msg](){Serial.println("MenuOrganizer::Notify - onAccept - " + msg); gui.SetMode(MODE_MENU); });
  notification.OnDecline(0, [msg](){ Serial.println("MenuOrganizer::Notify - onDecline - " + msg); gui.SetMode(MODE_MENU); });  
}

void MenuOrganizer::Notify(String msg, std::function<void()> onAccept, std::function<void()> onDecline){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(0, msg, {64, 32-5}, 0);   
  notification.OnAccept(0, onAccept);
  notification.OnDecline(0, onDecline); 
}


void MenuOrganizer::Notify(int level, String msg, int timeout){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(level, msg, {64, 32-5}, timeout);   
  notification.OnAccept(level, [msg](){Serial.println("MenuOrganizer::Notify2 - onAccept - " + msg); gui.SetMode(MODE_MENU); });
  notification.OnDecline(level, [msg](){Serial.println("MenuOrganizer::Notify2 - onDecline - " + msg); gui.SetMode(MODE_MENU); });  
}

void MenuOrganizer::Notify(int level, String msg, std::function<void()> onAccept, std::function<void()> onDecline){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(level, msg, {64, 32-5}, 0);   
  notification.OnAccept(level, onAccept);
  notification.OnDecline(level, onDecline); 
}

void MenuOrganizer::Notify(bool res){
  gui.SetMode(MODE_NOTIFICATION);
  if(res == true){
    notification.Set(0, "Success\n:)", {64, 32-5}, NOTIFICATION_TIME);
  }else{
    notification.Set(0, "Failed...\n:(", {64, 32-5}, 0);
  }
  notification.OnAccept(0, [](){gui.SetMode(MODE_MENU);});
  notification.OnDecline(0, [](){gui.SetMode(MODE_MENU);});  
}


void MenuOrganizer::Init(){  
  mainMenu->parent = mainMenu;
  dirMenu->parent = mainMenu;
  manageFilesMenu->parent = mainMenu;
  
  Serial.println("MenuOrganizer::Init - 1");
  AddOption(mainMenu, MenuOption{"Read card\0", true, [this](){
      rfid.RequestReadCard();
      
      Notify(">Reading<\nWaiting for card...", 0);
      notification.SetConditionalFunction(0, [rfid](){ return rfid.WasCardPresented(); });      
      //notification.OnDecline(0, [this](){Notify(1, "Cancelled", 500);});

      
      rfid.OnCardPresented([](){Serial.println("mainMenu_Read_card_option_lambda_function - Rfid card presented."); notification.Update(); });
      rfid.OnSuccess([this](){
          Serial.println("mainMenu_Read_card_option_lambda_function - Rfid read successfully.");
          SetOptionName(mainMenu, 0, "Read another card\0");
          //MenuOption readCard = mainMenu->options->get(0); strcpy(readCard.text, "Read another card\0"); mainMenu->options->set(0, readCard);
          ActivateOption(mainMenu, 1);
          ActivateOption(mainMenu, 2);
          ActivateOption(mainMenu, 3);
          
          Notify("Success\n:)", NOTIFICATION_TIME);
          //notification.SetConditionalFunction([notification](){
            //return notification.GetLifeSpan() > 2000; // calls onAcceptFunction after 5 secs
            //});
      });     
      rfid.OnFail([this](){
        Serial.println("mainMenu_Read_card_option_lambda_function - Rfid card read failed.");
        Notify("Failed reading\nCard may be incompatible...\n:(", 0);
        });  

   }});

/*
  AddOption(mainMenu, MenuOption{"Load from memory\0", true, [this](){        
        ClearOptions(dirMenu);
        Dir dir = SPIFFS.openDir("/rfid");
        int count = 0;
        while(dir.next() && count < MAX_FILES) {
          MenuOption opt = MenuOption();
          strcpy(opt.text, dir.fileName().c_str()+6);
          opt.text[dir.fileName().length()-10] = '\0';
          opt.active = true;
          String file_name = opt.text;
          opt.function = [this, file_name](){
                // use its name to open the file
                Serial.print("Load from memory - dir option picked function - name: "); Serial.println(file_name);
                int res = rfid.ReadFromFile((char *)file_name.c_str());
                
                //DrawResult(res);
                if(res == true){
                  ActivateOption(mainMenu, 2);
                  ActivateOption(mainMenu, 3);
                  DeactivateOption(mainMenu, 4);
                  Notify("Loaded successfully\n:)", NOTIFICATION_TIME);
                }else{
                  Notify("Failed to load " + file_name + "\n:(", 0);
                }
                
                SetMenu(mainMenu);
              };
          AddOption(dirMenu, opt);
          count++;
        }
        SetMenu(dirMenu);
  }});
*/    
  
  Serial.println("MenuOrganizer::Init - 2");
  AddOption(mainMenu, MenuOption{"Write card\0", false, [this](){
      rfid.RequestWriteCard();
      
      Notify("<Writing>\nWaiting for card...", 0);
      notification.SetConditionalFunction(0, [rfid](){ return rfid.WasCardPresented(); });      
      
      rfid.OnCardPresented([](){Serial.println("mainMenu_Read_card_option_lambda_function - Rfid card presented."); notification.Update(); });
      rfid.OnSuccess([this](){
          Serial.println("mainMenu_Read_card_option_lambda_function - Rfid wrote successfully.");
          Notify(1, "Success\nRfid wrote successfully\n:)", 500);
      });     
      rfid.OnFail([this](){
        Serial.println("mainMenu_Read_card_option_lambda_function - Rfid card write failed.");
        Notify(1, "Failed writing\nCard may be incompatible...\n:(", 0);
        });  
  }});

  AddOption(mainMenu, MenuOption{"Dump to serial\0", false, [this](){
      Notify("Open serial monitor\nand press ok...", 0);
      notification.OnAccept(0, [this, rfid](){rfid.DumpToSerial(); Notify(1, "Success\nCheck serial monitor\n:)", 1000);});          
      //Notify("Open serial monitor\nand press ok...", 
      ///*onAccept*/ [this, rfid](){rfid.DumpToSerial(); Notify(1, "Success\nCheck serial monitor\n:)", 1000);}, 
      ///*onDeclined*/ [this](){Notify(1, "Cancelled", 500);}
      //);
    }
  });

  AddOption(mainMenu, MenuOption{"Save to memory\0", false, [this](){
      gui.SetMode(MODE_NAME_PICKER);
      namePicker.OnSuccess([this](){
        Serial.println("MenuOrganizer::Init - namePicker.OnSuccess");
          Serial.print("FILENAME to write to: "); Serial.println(namePicker.GetName());
          String s = namePicker.GetName();
          rfid.SaveToFile(s);
          Notify("Saved as: "+s+"\n(/rfid/"+s+".txt)", 0);
        });

      namePicker.OnCancel([this](){
          Serial.println("MenuOrganizer::Init - namePicker.OnCancel");
          gui.SetMode(MODE_MENU);
        });
    }
  });

  AddOption(mainMenu, MenuOption{"Files\0", true, [this](){        
        ClearOptions(manageFilesMenu);
        Dir dir = SPIFFS.openDir("/rfid");
        int count = 0;
        while(dir.next() && count < MAX_FILES) {
          MenuOption opt = MenuOption();
          strcpy(opt.text, dir.fileName().c_str()+6);
          opt.text[dir.fileName().length()-10] = '\0';
          opt.active = true;
          String file_name = opt.text;
          opt.function = [this, file_name](){
                // use its name to open the file
                Serial.print("Files - dir option picked function - name: "); Serial.println(file_name);             
                // get current horizontal option for this particular option and execute its function
                MenuOption mho = menu->horizontal_options->get(menu->current_option.x);
                mho.function();    
              };
          AddOption(manageFilesMenu, opt);
          count++;
        }
          
        //add horizontal options
        AddHorizontalOption(manageFilesMenu, MenuOption{"Load", true, [this](){
                Serial.println("manageFilesMenu - Horizontal Load option picked.");              
                String file_name = String(menu->options->get(menu->current_option.y).text);               
                if(rfid.ReadFromFile((char *)file_name.c_str())){
                  SetOptionName(mainMenu, 0, "Read another card\0");
                  ActivateOption(mainMenu, 1);
                  ActivateOption(mainMenu, 2);
                  DeactivateOption(mainMenu, 3);
                  Notify("Loaded successfully\n:)", NOTIFICATION_TIME);
                  SetMenu(mainMenu);
                }else{
                  Notify("Failed to load " + file_name + "\n:(", 0);
                }
        }});

        AddHorizontalOption(manageFilesMenu, MenuOption{"Overwrite", true, [this](){
            Serial.println("manageFilesMenu - Horizontal Overwrite option picked.");
            String file_name = String(menu->options->get(menu->current_option.y).text);
            String read_source = rfid.GetLastReadSourceType();
            if(read_source != "Empty"){
              Notify("Are you sure you want\nto overwrite " + file_name + "\nusing " + read_source + " data?" , 
              /*onAccept*/ [this, file_name, rfid, read_source](){
                    if(rfid.SaveToFile(file_name)){
                      Notify(1, file_name+" file has been overwritten\nusing " + read_source +  " data.", 0);
                    }else{
                      Notify(1, file_name+" could not be overwritten\nfor some reason\nusing" + read_source + " data.", 0);
                    }
                },
              /*onDeclined*/ [this](){
                  gui.SetMode(MODE_MENU);
                }
              ); 
            }else{
              Notify("Rfid data buffer is empty.\nRead card or load file first.", 0);
            }
        }});
        
        AddHorizontalOption(manageFilesMenu, MenuOption{"Rename", true, [this](){
              Serial.println("manageFilesMenu - Horizontal Rename option picked.");
              namePicker.SetInitialName(menu->options->get(menu->current_option.y).text);
              gui.SetMode(MODE_NAME_PICKER);
              namePicker.OnSuccess([this](){
                Serial.println("manageFilesMenu - Rename option - namePicker.OnSuccess");
                  String new_name = namePicker.GetName();
                  String old_name = String(menu->options->get(menu->current_option.y).text);
                  if(files.Rename(old_name , new_name)){
                    SetOptionText(menu, menu->current_option.y, new_name);
                    Notify("Renamed " + old_name + "\ninto " + new_name + "\n(/rfid/" + new_name + ".txt)", 0);
                  }else{
                    Notify("Could not rename " + old_name + "\ninto " + new_name + "\n(/rfid/" + new_name + ".txt)", 0);
                  }
                });
              
              namePicker.OnCancel([this](){
                    Serial.println("manageFilesMenu - Rename option - namePicker.OnCancel (quit name picker)" );
                    gui.SetMode(MODE_MENU);
                });
        }});

        AddHorizontalOption(manageFilesMenu, MenuOption{"Delete", true, [this](){
            Serial.println("manageFilesMenu - Horizontal Delete option picked.");
            String file_name = String(menu->options->get(menu->current_option.y).text);
            Notify("Are you sure you want\nto delete " + file_name + " file?", 
            /*onAccept*/ [this, file_name](){
              if(files.Remove(file_name)){
                Notify(1, "Removed " + file_name + "\n(/rfid/" + file_name + ".txt)", 0);
                RemoveOption(menu, menu->current_option.y);
                ShiftCurrentOption(BUTTON_UP);
              }else{
                Notify(1, "Could not remove " + file_name + "\nfor some reason.\n(/rfid/" + file_name + ".txt)", 0);
              }}, 
            /*onDeclined*/ [this](){
                gui.SetMode(MODE_MENU);
              }
            );
        }});
                
        SetMenu(manageFilesMenu);
  }});

  /*
  AddOption(mainMenu, MenuOption{"example\0", true, [this](){
    
    }
  });
  */

  SetMenu(mainMenu);
}



void MenuOrganizer::AddOption(Menu *_menu, MenuOption option){
  _menu->len++;
  if(option.active == true){
    _menu->active_len++;
  }
  _menu->options->add(option);
}

void MenuOrganizer::RemoveOption(Menu *_menu, int option_num){
  if(option_num < _menu->options->size()){
    _menu->len--;
    _menu->active_len--;
    _menu->options->remove(option_num);
  }
}

void MenuOrganizer::SetOptionText(Menu *_menu, int option_num, String new_name){
  if(option_num < _menu->options->size()){
    MenuOption mo = _menu->options->get(option_num);
    memset(mo.text, 0, MAX_MENU_TEXT); strcpy(mo.text, (char*)new_name.c_str());
    _menu->options->set(_menu->current_option.y, mo);
  }
}

void MenuOrganizer::AddHorizontalOption(Menu *_menu, MenuOption option){
  _menu->horizontal_options->add(option);
}

void MenuOrganizer::ClearOptions(Menu *_menu){
  _menu->len = 0;
  _menu->active_len = 0;
  _menu->options->clear();
  
  _menu->horizontal_options->clear();
  
  ResetMenuState(_menu);
}

void MenuOrganizer::SetMenu(Menu *new_menu){
  menu = new_menu;
}

void MenuOrganizer::ActivateOption(Menu *_menu, int num){
  if(_menu->options->get(num).active == false){
    _menu->active_len++;
    MenuOption mo = _menu->options->get(num);
    mo.active = true;
    _menu->options->set(num, mo);
  }   
}

void MenuOrganizer::DeactivateOption(Menu *_menu, int num){
  if(_menu->options->get(num).active == true){
    _menu->active_len--;
    MenuOption mo = _menu->options->get(num);
    mo.active = false;
    _menu->options->set(num, mo);
  }  
}

void MenuOrganizer::HandleControls(){
  //Serial.println("KEY PRESSED");
  //int key = buttons.GetLast();
  //if(key && buttons.Limiter(250, 125, 600)){  
  if(int key = buttons.GetLast(/*use limiter?*/ true)){
    if(key == BUTTON_YES){
      //active_menu[current_option.y].function();
      menu->selected = menu->current_option.y;
    }
    else if(key == BUTTON_NO){
      menu->selected = CANCELLED;
      //DeclineCurrentOption();
    }
    else{
      ShiftCurrentOption(key); // replace with menu vars
      //MenuDataDump("MenuOrganizer::HandleControls");
    }
  }
}

void MenuOrganizer::MenuDataDump(String src){
  Serial.print("\n" + src + " MenuOrganizer::MenuDataDump");
  char buff[300];
  sprintf(buff, "\nGUI::HandleMenuControls - menu data dump\nlen=%d\nactive_len=%d\ncurrent_option.x=%d\ncurrent_option.y=%d\nselected=%d\nmax_options_displayed=%d\nstarting_option=%d\ncurrent_option_relative=%d\n\0",
    menu->len,
    menu->active_len,
    menu->current_option.x,
    menu->current_option.y,
    menu->selected,
    menu->max_options_displayed,
    menu->starting_option,
    menu->current_option_relative
  );
   Serial.println(buff);
   Serial.println();
}


void MenuOrganizer::ShiftCurrentOption(int buttonID){
  switch(buttonID){
    case BUTTON_UP:
      menu->current_option.y--;      
      if(menu->current_option.y < 0){
        menu->current_option.y = menu->active_len - 1;
        menu->current_option_relative = menu->max_options_displayed - 1;
        if(menu->current_option_relative >= menu->active_len){menu->current_option_relative = menu->active_len -1;}

        menu->starting_option--;
        if (menu->starting_option < 0){
          menu->starting_option = menu->active_len - menu->max_options_displayed;
          if(menu->starting_option < 0){menu->starting_option = 0;}
        }
      }else{
        menu->current_option_relative--;
        if(menu->current_option_relative < 0){
          menu->current_option_relative = 0;
          menu->starting_option--;
          if (menu->starting_option < 0){
            menu->starting_option = menu->active_len - menu->max_options_displayed;
            if(menu->starting_option < 0){menu->starting_option = 0;}
            menu->current_option_relative = menu->max_options_displayed-1;
            if(menu->current_option_relative > menu->current_option.y){menu->current_option_relative = menu->current_option.y;}
          }
        }    
      }
      
      
      break;
    case BUTTON_DOWN:
      menu->current_option.y++;
      if(menu->current_option.y >= menu->active_len){
        menu->current_option.y = 0;  
        menu->starting_option = 0;
        menu->current_option_relative = 0;
      }
      else{
        menu->current_option_relative++;
        if(menu->current_option_relative >= menu->max_options_displayed){
          menu->current_option_relative = menu->max_options_displayed-1;
          menu->starting_option++;
          if (menu->starting_option > menu->active_len - menu->max_options_displayed){
            menu->starting_option = 0;
            menu->current_option_relative = 0;
          }
        }
      }
      //if(menu->current_option.y > 5){
        //starting_y_draw = font_size*-1 * (menu->current_option.y - 5);
      //}

      break;
    case BUTTON_LEFT:
      menu->current_option.x--;
      if(menu->current_option.x < 0){
        menu->current_option.x = menu->horizontal_options->size() - 1;
      }
      break;
    case BUTTON_RIGHT:
      menu->current_option.x++;
      if(menu->current_option.x >= menu->horizontal_options->size()){
        menu->current_option.x = 0;
      }
      break;
  }

  //char buff[100];
  //sprintf(buff, "menu->current_option.x = %d \nmenu->current_option.y = %d \nmenu_len = %d\nactive_menu_len = %d", menu->current_option.x, menu->current_option.y, menu_len, active_menu_len);
  //Serial.println(buff);
}




/*
void MenuOrganizer::InitMenu(Menu *_menu, int max_options_disp){
  // useless 

  _menu->len = 0;
  _menu->active_len = 0;
  _menu->current_option = {0,0};
  _menu->selected = INVALID;
  _menu->max_options_displayed_default = max_options_disp;
  _menu->max_options_displayed = max_options_disp;
  _menu->starting_option = 0;
  _menu->current_option_relative = 0;
}
*/



void MenuOrganizer::UpdateActiveMenu(){
  //memset(active_menu, 0, sizeof(Menu)); WRONG (LinkedList must be resetted explicitly)
  /*
  active_menu->len = 0;
  for(int i=0; i < menu->len; i++){
    if(menu->options->get(i).active == true){
      active_menu->len++;
      
      for(int c=0; c < MAX_MENU_TEXT-1; c++){
        active_menu->option[active_menu->len-1].text[c] = menu->options->get(i).text[c];
      }
      active_menu->option[active_menu->len-1].text[MAX_MENU_TEXT-1] = '\0';
      active_menu->option[active_menu->len-1].function = menu->options->get(i).function;
    }   
  }

  if(active_menu->current_option.y >= active_menu->len){
    active_menu->current_option.y = active_menu->len - 1;
  }

  selected = INVALID;
  menu->starting_option = 0;
  menu->current_option_relative = 0;
  if(active_menu_len < max_options_displayed_default){
    menu->max_options_displayed = active_menu->len;
  }
  else{
    menu->max_options_displayed = menu->max_options_displayed_default;
  }

  */
}





























/*
mainMenu = {
  5, //len
  2, //active_len
  0,0, // current option
  INVALID, //selection
  5, //max_options_displayed_default
  5, //max_options_displayed
  0, //starting_option
  0, //current_option_relative

  
    "Read card\0", true, [](){
        gui.DrawNotification("> Reading <\nWaiting for the card...\0");
        if(WaitForCard_AllowCancel()){
          bool res = rfid.ReadCard();
          DrawResult(res);
          if(res == true){
            gui.ActivateOption(2);
            gui.ActivateOption(3);
            gui.ActivateOption(4);
            
          }
        }
      },
      
    "Load from memory", true, [](){
        MenuOption dirSelection[30];
        Dir dir = SPIFFS.openDir("/rfid");
        int count = 0;
        while(dir.next()) {
          strcpy(dirSelection[count].text, dir.fileName().c_str()+6);
          dirSelection[count].text[dir.fileName().length()-10] = '\0';
          dirSelection[count].active = true;
          count++;
        }
        gui.SetMenu(dirSelection, count);
        while(true){
          gui.HandleMenuControls();
          if(gui.WasSelected()){
            int choice = gui.GetSelection();
            if(choice != CANCELLED){
              //dirSelection[choice].text is the chosen file name
              int res = rfid.ReadFromFile(dirSelection[choice].text);
              DrawResult(res);
              if(res == true){
                mainMenu[2].active = true; gui.ActivateOption(2);
                mainMenu[3].active = true; gui.ActivateOption(3);
                mainMenu[4].active = false; gui.DeactivateOption(4);
              }
              //Serial.println(dirSelection[choice].text);
              //rfid.DumpFromFile(dirSelection[choice].text);
              break;
            }
            else{
              break;
            }
            gui.ResetSelection();
          }
          gui.Draw();
        }
        //gui.Set(mainMenu, mainMenuLen);
      },
      
    "Write card\0", false, [](){
        gui.DrawNotification("< Writing >\nWaiting for the card...\0");
        if(WaitForCard_AllowCancel()){
          bool res = rfid.WriteCard();
          DrawResult(res);
        }
      },
      
    "Dump to serial\0", false, [](){
        rfid.DumpToSerial();
        DrawResult(true);
      },
      
    "Save to memory\0", false, [](){
        gui.GetNamePrompt();
      },
      
    "Manage files\0", true, [](){
      
      },
    
};
*/
