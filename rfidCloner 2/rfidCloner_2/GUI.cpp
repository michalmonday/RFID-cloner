#include "GUI.h"

SSD1306Wire display(0x3c, D1, D2); // D1 D2

#include "images\intro.h"


GUI::GUI(){
}

void GUI::Init(){
  Serial.println("GUI::Init");
  display.init();
  display.flipScreenVertically();
  display.setBrightness(settings.Get("Brightness").toInt());
    
  Serial.println("MenuOrganizer::Init");
  menuOrganizer.Init();
  Serial.println("MenuOrganizer::Init - end");
  
  namePicker.Reset();

  init_time = millis();
  gui_mode = MODE_INTRO;
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

void GUI::DrawBacklight(int x, int y, int width, int font_size){
  display.fillRect(x, y+1, width, font_size+1);
  int circle_y = y+font_size/2+1;
  int circle_radius = font_size/2;
  display.fillCircle(x, circle_y, circle_radius); display.fillCircle(x + width, circle_y, circle_radius); // make it rounded
}

void GUI::DrawMenu(){
  display.setFont(ArialMT_Plain_10);
  byte font_size = 10;
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  LinkedList<String> names = menuOrganizer.GetOnScreenOptionNames();// and simply draw them, possibly use linked list for that 
  int cur_opt = 0;
  cur_opt = menuOrganizer.GetCurrentOnScreenOption();

  int x = font_size/2 + 2;
  if(int scrollbar_shift = DrawScrollIndicator(menuOrganizer.GetActiveOptionsCount(), menuOrganizer.GetMaxOptionsDisplayed(), menuOrganizer.GetStartingOptionIndex())){
    x += scrollbar_shift + font_size/5;
  }
  
  for(int i = 0; i < names.size(); i++)
  { 
    int y = i*(font_size + font_size/5) + 1;
    
    String txt = names.get(i);
    if(i == cur_opt){
      DrawBacklight(x, y, display.getStringWidth(names.get(i)), font_size);

      display.setColor(BLACK);
      display.drawString(x, y, txt);
      display.setColor(WHITE);

      if(menuOrganizer.HasHorizontalOptions()){
        display.drawString(x + display.getStringWidth(txt) + int(font_size*0.8), y-1, "[" + menuOrganizer.GetHorizontalOptionName() + "]");
      }
    }else{
      display.drawString(x, y, txt);
    }    
  }
}

// return amount of pixels taken horizontally
int GUI::DrawScrollIndicator(int items_count, int max_items_displayed, int starting_item_index){
  //Serial.println("GUI::DrawScrollIndicator - items_count = " + String(items_count)+ "; max_items_displayed = " + String(max_items_displayed) + "; starting_item_index = " + String(starting_item_index));
  if(items_count > max_items_displayed){
    int width = 5;
    display.drawRect(0,0, width, 64);
    uint16_t height = (uint16_t)int(float(max_items_displayed) / float(items_count) * 62.0);
    uint16_t y_pos = (uint16_t)int(64.0 / float(items_count) * float(starting_item_index));
    display.fillRect(1, y_pos+1, width-1, height+1);
    return width;
  }
  return 0;
}


void GUI::DrawRfidBuffer(){
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  uint8_t rect_padding = 3; 
  uint8_t font_size = 10;
  
  String read_source = rfid.GetLastReadSourceType();
  DrawStringInRect(read_source, 128, 64, rect_padding, font_size, TEXT_ALIGN_RIGHT);
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

/*
int brightness_add = 1; 
int brightness = 0;
*/


void GUI::Update(){
  display.clear();
  
  //display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(128, 0, String(system_get_free_heap_size()));

  switch(gui_mode){
    case MODE_INTRO:
      display.drawXbm((128-intro_width) / 2, (64 - intro_height) / 2, intro_width, intro_height, intro_bits);
      if(millis() - init_time > INTRO_LENGTH){
        gui_mode = MODE_MENU;
      }
      break;
    
    case MODE_MENU:
      DrawMenu();
      if(menuOrganizer.AllowedBufferDraw()){
        DrawRfidBuffer();
      }
      menuOrganizer.Update();
      break;
      
    case MODE_NAME_PICKER:
      DrawNamePicker();
      namePicker.Update();
      break;
      
    case MODE_NOTIFICATION:
      DrawNotification();
      notification.Update();
      break;
      
    case MODE_BUTTON_CALIBRATION_VIEW:
      DrawButtonsCalibration();
      break;

    case MODE_PROGRESS_BAR:
      DrawProgressBar();
      progressBar.Update();
      break;
  }

/*
  brightness += brightness_add;
  if(brightness > 255 || brightness < 0){brightness_add *= -1; brightness += brightness_add;}
  display.setBrightness(brightness);

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 0, String(brightness));
*/

  display.display();
}

void GUI::DrawButtonsCalibration(){
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  for(int i=0; i<BUTTON_COUNT; i++){
    int val_current = int(buttons.GetButtonExpectedRead(i));
    int val_initial = int(buttons.GetButtonInitialExpectedRead(i));
    String n = buttons.GetButtonName(i);
    
    display.drawString(0, i*10 + 1, n + " - " + val_current + ":" + val_initial);
  }

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 0, "A0 = " + String(buttons.GetLastAnalogInput()));
  display.drawString(128, 12, String(buttons.GetLastButtonName()));
}

void GUI::DrawProgressBar(){
  String label = progressBar.GetLabel();
  if(label.length()){
    int val = progressBar.GetValue();
    int progress = progressBar.GetProgressPercent();
    int low_border = progressBar.GetLowerBorder();
    int high_border = progressBar.GetHigherBorder();

    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawProgressBar((128 / 4) / 2, 64/3*2, 128 / 4 * 3, 10, progress);
    display.drawString(64, 64/3, "Brightness = " + String(val));
  }
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


void GUI::SetBrightness(int val){
  display.setBrightness(val);
}










