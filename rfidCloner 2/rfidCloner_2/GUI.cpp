#include "GUI.h"

SSD1306Wire display(0x3c, D1, D2); // D1 D2







GUI::GUI(){
}

void GUI::Init(){
  Serial.println("GUI::Init");
  display.init();
  display.flipScreenVertically();
    
  Serial.println("MenuOrganizer::Init");
  menuOrganizer.Init();
  Serial.println("MenuOrganizer::Init - end");
  
  namePicker.Reset();
}
/*
void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    //display.setFont(ArialMT_Plain_10);
    display.setFont(ArialMT_Plain_24);
    for(int y=0;y < 5; y++)
    {
      //display.drawString(0, y*10, "Hello world");
    }

    int inputVal = analogRead(A0); // Analog Values 0 to 1023
    char buff[10] = {0};
    sprintf(buff, "%d", inputVal);
    display.drawString(0, 10, buff);

  //display.setTextAlignment(TEXT_ALIGN_CENTER);
  //display.drawString(64, 22, "Center aligned (64,22)");
}

void drawTextFlowDemo() {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 0, 128,
      "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}
*/


void GUI::DrawNotification(){
  if(notification.IsAvailable()){
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.setFont(ArialMT_Plain_10);
    XY pos = notification.GetPos();
    display.drawString(64, 32, notification.GetMsg()); 
      
  }
}

/*
void GUI::DrawMenu(){
  Serial.println("GUI::DrawMenu begining");delay(20);
  display.setFont(ArialMT_Plain_10);
  Serial.println("GUI::DrawMenu begining2");delay(20);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  Serial.println("GUI::DrawMenu begining3");delay(20);

  LinkedList<String> names = menuOrganizer.GetOnScreenOptionNames();// and simply draw them, possibly use linked list for that 
  Serial.println("GUI::DrawMenu begining4");delay(20);
  int cur_opt = 0;
  Serial.println("GUI::DrawMenu begining5");delay(20);
  cur_opt = menuOrganizer.GetCurrentOnScreenOption();

  char buff[100]={0};
  sprintf(buff, "GUI::DrawMenu names length = %d", names.size());
  Serial.println(buff);

  Serial.println("GUI::DrawMenu - 2nd (i=1) option size " + String(names.get(1).length())); 
  Serial.println("GUI::DrawMenu - free heap size " + String(system_get_free_heap_size()));
  for(int i = 0; i < names.size(); i++)
  { 
    Serial.println("GUI::DrawMenu - option " + String(i) + " name:" + names.get(i) + " size:" + String(names.get(i).length())); delay(20);
    if(i == cur_opt){
      //char buff[50];
      //sprintf(buff, "-- %s --", names.get(i).c_str());
      Serial.println("GUI::DrawMenu - 1"); 
      display.drawString(64, i*10, "-- " + names.get(i) + " --");
      Serial.println("GUI::DrawMenu - 2");
    }else{
      Serial.println("GUI::DrawMenu - 3"); 
      display.drawString(64, i*10, names.get(i));
      Serial.println("GUI::DrawMenu - 4"); 
    }
  }
  Serial.println("GUI::DrawMenu - end"); 
  //display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(128, 0, String(millis()));
}
*/

void GUI::DrawMenu(){
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  LinkedList<String> names = menuOrganizer.GetOnScreenOptionNames();// and simply draw them, possibly use linked list for that 
  int cur_opt = 0;
  cur_opt = menuOrganizer.GetCurrentOnScreenOption();

  int x_shift_scroll_indicator = 4 + DrawScrollIndicator(menuOrganizer.GetActiveOptionsCount(), menuOrganizer.GetMaxOptionsDisplayed(), menuOrganizer.GetStartingOptionIndex());
  for(int i = 0; i < names.size(); i++)
  { 
    int y = i*(10+2) + 1;
    
    String txt = "";
    if(i == cur_opt){
      if(menuOrganizer.HasHorizontalOptions()){
        //txt = names.get(i) + " ";
        //DrawStringInRect(menuOrganizer.GetHorizontalOptionName() , display.getStringWidth(txt) + 3, y, /*rect_padding*/ 2, /*font_size*/ 10, TEXT_ALIGN_LEFT);

        txt = names.get(i) + " <= [" + menuOrganizer.GetHorizontalOptionName() + "]";
      }else{
        txt = names.get(i) + " <=";
      }
    }else{
      txt = names.get(i);
    }
    display.drawString(x_shift_scroll_indicator, y, txt);
  }

  //display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(128, 0, String(millis()));
}

// return amount of pixels taken horizontally
int GUI::DrawScrollIndicator(int items_count, int max_items_displayed, int starting_item_index){
  //Serial.println("GUI::DrawScrollIndicator - items_count = " + String(items_count)+ "; max_items_displayed = " + String(max_items_displayed) + "; starting_item_index = " + String(starting_item_index));
  if(items_count > max_items_displayed){
    int width = 5;
    display.drawRect(0,0, width, 64);
    uint16_t height = (uint16_t)int(float(max_items_displayed) / float(items_count) * 62.0);
    uint16_t y_pos = (uint16_t)int(64.0 / float(items_count) * float(starting_item_index));
    display.fillRect(1, y_pos+1, width-1, height);
    return width;
  }
  return 0;
}


void GUI::DrawRfidBuffer(){
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  uint8_t rect_padding = 3; 
  uint8_t font_size = 10;

  
  
  /*
  //unsigned long last_read_time = rfid.GetLastReadTime();
  if(last_read_time > 0){
    
    //String str_time_passed = GetTimeSince(last_read_time);
    int src = rfid.GetLastReadSourceType();
    if(src == READ_SOURCE_CARD){
      file_name = "Card";
    }else if(src == READ_SOURCE_FILE){
      file_name = rfid.GetLastReadFileName() + " file";
    }
  }else{
    file_name = "Empty";
  }
  */

  String read_source = rfid.GetLastReadSourceType();


  DrawStringInRect(read_source, 128, 64, rect_padding, font_size, TEXT_ALIGN_RIGHT);

/*
  uint8_t padding = 3; 
  uint8_t font_size = 10;
  uint16_t width = display.getStringWidth(file_name);
  //display.drawString(128, 64-font_size*2-padding*3, "DATA:");
  display.drawString(128-padding, 64-font_size-padding-1, file_name);
  display.drawRect(128 - width - padding*2, 64-font_size-padding*2, width+padding*2, 10+padding*2);
*/
  //DrawCenteredString(file_name, 10, 128, 32);
}


void GUI::DrawStringInRect(String txt, int x, int y, int padding, int font_size, OLEDDISPLAY_TEXT_ALIGNMENT alignment){
  // alignment adjusted specifically for this projects menu (left/right/center use different part of the drawn object as a reference for starting/ending point, and has small different adjustments)
  uint16_t width = display.getStringWidth(txt);
  if(alignment == TEXT_ALIGN_RIGHT){
    display.drawString(x-padding, y-font_size-padding-1, txt);
    display.drawRect(x - width - padding*2, y-font_size-padding*2, width+padding*2, font_size+padding*2);
  }else if(alignment == TEXT_ALIGN_LEFT){
    display.drawString(x, y, txt);
    display.drawRect(x - padding, y-padding+1, width+padding*2, font_size+padding*2 + 1);    
  }
}

/*
void GUI::DrawCenteredString(String str, int font_size, int x, int y){
  display.drawString(x, y - ((font_size/2+2)*(GetNewLineCount(str)+1)), str);  
}
*/

void GUI::DrawNamePicker(){
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, (int)((64.0/3.0)-5.0), "Pick a name");
  
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  
  int x = namePicker.GetXpos();
  String n = namePicker.GetName();
  String firstPart = n.substring(0, x);
  String drawnName(">");
  drawnName.concat(firstPart);
  drawnName.concat("|");
  drawnName.concat(n.substring(x));

  display.drawString(64, (int)(((64.0/3.0)*2.0)-8.0), drawnName);
}


void GUI::Update(){
  display.clear();
  
  //display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(128, 0, String(system_get_free_heap_size()));

  if(gui_mode == MODE_MENU){
    DrawMenu();
    DrawRfidBuffer();
    menuOrganizer.Update();
  }else if(gui_mode == MODE_NAME_PICKER){
    DrawNamePicker();
    namePicker.Update();
  }else if(gui_mode == MODE_NOTIFICATION){
    DrawNotification();
    notification.Update();
  }
  display.display();
}



/*
void GUI::Update(){
  display.clear();

  if(gui_mode == MODE_MENU){
    Serial.println("GUI::Update - Before menuOrganizer.Update1");delay(20);
    //menuOrganizer.MenuDataDump("GUI::Update");
    Serial.println("GUI::Update - Before menuOrganizer.Update2");delay(20);
    menuOrganizer.Update();
    Serial.println("GUI::Update - After menuOrganizer.Update1");delay(20);
    //menuOrganizer.MenuDataDump("GUI::Update");
    Serial.println("GUI::Update - After menuOrganizer.Update2");delay(20);
    DrawMenu();
    Serial.println("GUI::Update - After DrawMenu");delay(20);
  }else if(gui_mode == MODE_NAME_PICKER){
    Serial.println("GUI::Update - Before namePicker.Update");delay(20);
    namePicker.Update();
    Serial.println("GUI::Update - After namePicker.Update");delay(20);
    DrawNamePicker();
    Serial.println("GUI::Update - After DrawNamePicker");delay(20);
  }else if(gui_mode == MODE_NOTIFICATION){
    Serial.println("GUI::Update - Before notification.Update");delay(20);
    notification.Update();
    Serial.println("GUI::Update - After notification.Update");delay(20);
    DrawNotification();
    Serial.println("GUI::Update - After DrawNotification");delay(20);
  }
  
  
  display.display();
}
*/












