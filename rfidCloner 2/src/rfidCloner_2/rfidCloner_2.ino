
#include "MenuOrganizer.h"
MenuOrganizer menuOrganizer;

#include "GUI.h"
GUI gui; 
/*
  [OLED -> NodeMCU]
    SDA -> D1 
    SCL -> D2
 */

#include "Rfid.h"
Rfid rfid;
/*
  n  [RC522 -> NodeMCU]
     RST -> D3
     SDA -> D8
    MOSI -> D7
    MISO -> D6
    SCK  -> D5
 */

#include "Buttons.h"
Buttons buttons;

/*
Button connections:
NodeMCU 3.3.V pin connection goes TWICE to the following 2 legs of buttons:
// Resistors are connected in series (to each other) and to 1 leg of each button.
// Starting at 3.3V and going through: 4.7k -> 2.2K -> 1K -> 680 -> 470 -> 220 -> A0 -> 1k -> GND.
// Other leg of each button is connected directly to 3.3V in parallel. This way different
// voltage is provided depending on the button pressed.
4.7k - up
2.2k - down
1k - left
680 - right
470 - accept
220 - reject
NodeMCU A0 pin connection
1k - ground
*/

#include "NamePicker.h"
NamePicker namePicker;

#include "Notification.h"
Notification notification;

#include "Files.h"
Files files;

#include "PCinterface.h"
PCinterface pcInterface;

#include "ProgressBar.h"
ProgressBar progressBar;

#include "Settings.h"
Settings settings;

#include "Lock.h"
Lock lock;

void setup() {
  Serial.begin(115200);
  //delay(10000);
  Serial.println("Starting");

  buttons.Init();
  files.Init();
  //files.Format();
  //files.DumpDirToSerial("/");
  //files.DumpDirToSerial("/rfid");
  //files.RemoveDir("/rfid");

  gui.Init();
  rfid.Init();
  notification.Init();

  settings.Load();            // see Settings.cpp to see default settings
}

void loop() {
  buttons.Update();
  gui.Update();
  rfid.Update();
  
}






















