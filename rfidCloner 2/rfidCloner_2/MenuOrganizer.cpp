#include "MenuOrganizer.h"

#include "GUI.h"
extern GUI gui;

#include "Lock.h"
extern Lock lock;

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
    names.add(screen_options.get(i).text);
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
  mainMenu->parent = mainMenu; mainMenu->allow_rfid_buffer_draw = true;
  manageFilesMenu->parent = mainMenu;
  pcMenu->parent = mainMenu;
  settingsMenu->parent = mainMenu;
  debugMenu->parent = settingsMenu;
  infoMenu->parent = debugMenu;

  
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

  AddOption(mainMenu, MenuOption{"Dump to serial", false, [this](){
      Notify("Open serial monitor\nand press ok...", 0);
      notification.OnAccept(0, [this, rfid](){rfid.DumpToSerial(); Notify(1, "Success\nCheck serial monitor\n:)", 1000);});          
      //Notify("Open serial monitor\nand press ok...", 
      ///*onAccept*/ [this, rfid](){rfid.DumpToSerial(); Notify(1, "Success\nCheck serial monitor\n:)", 1000);}, 
      ///*onDeclined*/ [this](){Notify(1, "Cancelled", 500);}
      //);
    }
  });

  AddOption(mainMenu, MenuOption{"Save to memory", false, [this](){
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

  AddOption(mainMenu, MenuOption{"Files", true, [this](){        
        ClearOptions(manageFilesMenu);
        Dir dir = SPIFFS.openDir("/rfid");
        int count = 0;
        while(dir.next() && count < MAX_FILES) {
          MenuOption opt = MenuOption();
          String path = dir.fileName();
          opt.text = path.substring(6, path.indexOf(".txt"));
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
                String file_name = menu->options->get(menu->current_option.y).text;               
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
            String file_name = menu->options->get(menu->current_option.y).text;
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
                  String old_name = menu->options->get(menu->current_option.y).text;
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
            String file_name = menu->options->get(menu->current_option.y).text;
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
  AddOption(mainMenu, MenuOption{"PC interface\0", true, [this](){
        ClearOptions(pcMenu);
        AddOption(pcMenu, MenuOption{"Send card\0", true, [this](){
          pcInterface.SendCard(String("college"));
          }});        
        SetMenu(pcMenu);      
  }});
  */
  
  /*
  AddOption(mainMenu, MenuOption{"example\0", true, [this](){
    
    }
  });
  */
  
  AddOption(mainMenu, MenuOption{"Settings", true, [this](){
      ClearOptions(settingsMenu);

      AddOption(settingsMenu, MenuOption{"Brightness", true,[this](){
          progressBar.SetLabel("Brightness");
          progressBar.SetBorders(0, 255);
          progressBar.SetValue(settings.Get("Brightness").toInt());
          progressBar.OnSuccess([this](){
            // save it (eeprom or spiffs file, create settings.cpp / settings.h)
            int val = progressBar.GetValue();
            gui.SetBrightness(val);
            settings.Set("Brightness", val);
            Notify("Brightness has been\nset to " + String(val), 1000);
            });
            
          progressBar.OnFail([this](){
            gui.SetMode(MODE_MENU);
            });
            
          progressBar.SetValGetter([this](int key){
            int to_add = 0;
            switch(key){
              case BUTTON_RIGHT:
                to_add = 1;
                break;
              case BUTTON_LEFT:
                to_add = -1; 
                break;  
              case BUTTON_UP:
                to_add = 10;
                break;
              case BUTTON_DOWN:
                to_add = -10;
                break;
              default:
                return progressBar.GetValue();
                break;
            }
            int brightness = progressBar.GetValue() + to_add;
            gui.SetBrightness(brightness);
            return brightness;
            });
  
          gui.SetMode(MODE_PROGRESS_BAR);
        }
      });

      AddOption(settingsMenu, MenuOption{"Lock", true,[this](){
          Notify("Set lock state:\nAccept - active\nDecline - not active", 
            /*onAccept*/ [this](){
                settings.Set("UseLock", 1);
                Notify(1, "Lock is turned on.", 0);
              },
            /*onDecline*/ [this](){
                settings.Set("UseLock", 0);
                Notify(1, "Lock is turned off.", 0);
              }
          );

      }});

      AddOption(settingsMenu, MenuOption{"Last read file name", true,[this](){
          Notify("Restore default?\n(Default = %Last_read%)", 
            /*onAccept*/ [this](){
                String new_name = "%Last_read%";
                String old_name = settings.Get("LastReadFileName");
                if(files.Rename(old_name , new_name)){
                  settings.Set("LastReadFileName", new_name);
                  Notify(1, "Renamed " + old_name + "\ninto " + new_name + "\n(/rfid/" + new_name + ".txt)", 0);
                }else{
                  Notify(1, "Could not rename " + old_name + "\ninto " + new_name + "\n(/rfid/" + new_name + ".txt)", 0);
                }
              },
            /*onDecline*/ [this](){
                gui.SetMode(MODE_NAME_PICKER);
                String old_name = settings.Get("LastReadFileName");
                namePicker.SetInitialName(old_name);
                namePicker.OnSuccess([this, old_name](){
                  Serial.println("settingsMenu - Last read file name option - namePicker.OnSuccess");
                    String new_name = namePicker.GetName();
                    if(files.Rename(old_name , new_name)){
                      settings.Set("LastReadFileName", new_name);
                      Notify(1, "Renamed " + old_name + "\ninto " + new_name + "\n(/rfid/" + new_name + ".txt)", 0);
                    }else{
                      Notify(1, "Could not rename " + old_name + "\ninto " + new_name + "\n(/rfid/" + new_name + ".txt)", 0);
                    }
                  });
                
                namePicker.OnCancel([this](){
                      Serial.println("settingsMenu - Last read file name option - namePicker.OnCancel (quit name picker)" );
                      gui.SetMode(MODE_MENU);
                  });
              }
          );

        
      }});
      
      
      AddOption(settingsMenu, MenuOption{"Debugging", true, [this](){
            ClearOptions(debugMenu);
            AddOption(debugMenu, MenuOption{"Buttons calibration view\0", true, [this](){
              Notify("Format:\nName\nCurrent expected A0 read\nInitial expected A0 read\n(Reboot required to exit)", 
                  /*onAccept*/[this](){gui.SetMode(MODE_BUTTON_CALIBRATION_VIEW);},
                  /*onDecline*/[this](){gui.SetMode(MODE_MENU);}
                  );
            }});

            
            AddOption(debugMenu, MenuOption{"Info", true, [this](){
              ClearOptions(infoMenu);
              
              AddOption(infoMenu, MenuOption{"Free heap: " + FormatBytes(ESP.getFreeHeap()), true, [this](){Notify("Free heap memory size.\n(Free dynamic memory)",0);}});
              AddOption(infoMenu, MenuOption{"Sketch size: " + FormatBytes(ESP.getSketchSize()), true, [this](){Notify("The size of the\ncurrent sketch.",0);}});
              AddOption(infoMenu, MenuOption{"Unused sketch: " + FormatBytes(ESP.getFreeSketchSpace()), true, [this](){Notify("The free sketch space.",0);}});
              AddOption(infoMenu, MenuOption{"Chip ID: " + String(ESP.getChipId()), true, [this](){Notify("Core version.",0);}});     
              AddOption(infoMenu, MenuOption{"Sdk version: " + String(ESP.getSdkVersion()), true, [this](){Notify("Version of software\ndevelopment kit.",0);}});
              AddOption(infoMenu, MenuOption{"CPU frequency: " + String(ESP.getCpuFreqMHz()) + " MHz", true, [this](){Notify("The CPU frequency in MHz.\nMega Hertz - millions of\noccurences within a second.",0);}});
              AddOption(infoMenu, MenuOption{"Fl chip ID: " + String(ESP.getFlashChipId()), true, [this](){Notify("The flash chip ID.",0);}});
              AddOption(infoMenu, MenuOption{"Fl chip sz: " + FormatBytes(ESP.getFlashChipSize()), true, [this](){Notify("The flash chip size,\nin bytes, as seen by\nthe SDK (may be less\nthan actual size).",0);}});
              AddOption(infoMenu, MenuOption{"Fl chip real sz: " + FormatBytes(ESP.getFlashChipRealSize()), true, [this](){Notify("The real chip size,\nin bytes, based on\nthe flash chip ID.",0);}});
                  
              //ESP.getCoreVersion(); 
              //ESP.getFlashChipSpeed(void);

              SetMenu(infoMenu);
            }});
        
            SetMenu(debugMenu);
          }});   
          
      SetMenu(settingsMenu);
    }
  });
  
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
    mo.text = new_name;
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




/*
https://arduino-esp8266.readthedocs.io/en/2.4.1/libraries.html#esp-specific-apis
ESP-specific APIs
Some ESP-specific APIs related to deep sleep, RTC and flash memories are available in the ESP object.
ESP.deepSleep(microseconds, mode) will put the chip into deep sleep. mode is one of WAKE_RF_DEFAULT, WAKE_RFCAL, WAKE_NO_RFCAL, WAKE_RF_DISABLED. (GPIO16 needs to be tied to RST to wake from deepSleep.)
ESP.rtcUserMemoryWrite(offset, &data, sizeof(data)) and ESP.rtcUserMemoryRead(offset, &data, sizeof(data)) allow data to be stored in and retrieved from the RTC user memory of the chip respectively. Total size of RTC user memory is 512 bytes, so offset + sizeof(data) shouldn’t exceed 512. Data should be 4-byte aligned. The stored data can be retained between deep sleep cycles. However, the data might be lost after power cycling the chip.
ESP.restart() restarts the CPU.
ESP.getResetReason() returns a String containing the last reset reason in human readable format.
ESP.getFreeHeap() returns the free heap size.
ESP.getChipId() returns the ESP8266 chip ID as a 32-bit integer.
ESP.getCoreVersion() returns a String containing the core version.
ESP.getSdkVersion() returns the SDK version as a char.
ESP.getCpuFreqMHz() returns the CPU frequency in MHz as an unsigned 8-bit integer.
ESP.getSketchSize() returns the size of the current sketch as an unsigned 32-bit integer.
ESP.getFreeSketchSpace() returns the free sketch space as an unsigned 32-bit integer.
ESP.getSketchMD5() returns a lowercase String containing the MD5 of the current sketch.
ESP.getFlashChipId() returns the flash chip ID as a 32-bit integer.
ESP.getFlashChipSize() returns the flash chip size, in bytes, as seen by the SDK (may be less than actual size).
ESP.getFlashChipRealSize() returns the real chip size, in bytes, based on the flash chip ID.
ESP.getFlashChipSpeed(void) returns the flash chip frequency, in Hz.
ESP.getCycleCount() returns the cpu instruction cycle count since start as an unsigned 32-bit. This is useful for accurate timing of very short actions like bit banging.
ESP.getVcc() may be used to measure supply voltage. ESP needs to reconfigure the ADC at startup in order for this feature to be available. Add the following line to the top of your sketch to use getVcc:
ADC_MODE(ADC_VCC);
TOUT pin has to be disconnected in this mode.
Note that by default ADC is configured to read from TOUT pin using analogRead(A0), and ESP.getVCC() is not available.  
*/


