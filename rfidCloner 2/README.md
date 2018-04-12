# Critical bug  
Please refrain from using "Lock" feature from "Settings" menu. I pushed update to github rather as a backup because I've had issues with PC and "Lock" is not ready yet. If you by some chance turned it on and can't get through the initial lock screen then the combination to enter is: BUTTON_UP - BUTTON_DOWN - BUTTON_LEFT - BUTTON_RIGHT - BUTTON_ACCEPT.  Sorry for inconvenience, I'll implement it properly soon.  


![](https://i.imgur.com/N4BjJwe.jpg)  

[Youtube demo video link](https://www.youtube.com/watch?v=K7hS6TLmSes)  

# List of contents
* [Parts used](#parts-used)  
* [Required libraries](#required-libraries)  
* [Wiring](#wiring)  
* [Credits](#credits) 
* [Pictures](#pictures)


# Parts used 
-NodeMCU (version with CP2102)  
-RC522 RFID reader module  
-SSD1306 OLED display (i2C - 4 pin version)  
-MIFARE 1K RFID card (with changeable UID, 13.56Mhz)  
-some RFID card to copy (only 13.56Mhz cards can be read/written by this particular card reader module)  
-buttons (6 of them)  
-resistors (4.7k, 2.2k, 2x1k, 680, 470, 220)  
-cables  
-switch  
-soldering equipment  
-hot glue (or tape)  
-3 AAA batteries (could be 3.7 li-po flat battery or any other battery having over 3.3V that doesn't provide more voltage than NodeMCU can accept using Vin pin)  
-2 pin dupont connector (for easier battery replacement/removal)    



# Required libraries  
-[rfid](https://github.com/miguelbalboa/rfid/)  
-[ESP8266 OLED SSD1306](https://github.com/ThingPulse/esp8266-oled-ssd1306/)  
-[LinkedList](https://github.com/ivanseidel/LinkedList)  


# Wiring

| NodeMCU | RFID-RC522 |
| --- | --- |
| D3 | RST |
| D5 | SPI SS |
| D6 | SPI MOSI |
| D7 | SPI MISO |
| D8 | SPI SCK |
| GND | GND |
| 3.3V | 3.3V |

| NodeMCU | SSD1306 128x64 OLED display |
| --- | --- |
| D1 | SDA |
| D2 | SCL |
| 3.3V | VCC |
| GND | GND |  

I didn't do it but in general it is good idea (e.g. for reliability) to connect SDA and SCL pins through 10K resistors to 3.3V.  

| NodeMCU | Parts | Battery |
| --- | --- | --- |
| Vin | Dupont 2-pin connector + Switch | V+ |
| GND | Dupont 2-pin connector | V- |
  
(The graph below shows that only 1 3.3V pin was used but other pins could also be used if that's more comfortable to solder. The same applies to GND pins. The battery must be connected to Vin pin though because it has higher voltage ~4.8V and has to go through regulator.)  
  
![](https://i.imgur.com/qnWrLDe.png)


# Credits  
The creators of libraries and boards/modules used in this project. RFID functionality is based on the [RFID library](https://github.com/miguelbalboa/rfid/) created by Miguel Balboa and other contributors. When trying to implement menu I was relying on the [esp8266_deauther](https://github.com/spacehuhn/esp8266_deauther/blob/master/esp8266_deauther/DisplayUI.cpp) project created by spacehuhn.

# Pictures  
![](https://i.imgur.com/2CDCzr6.jpg)  
![](https://i.imgur.com/7RNlH2X.jpg)  
![](https://i.imgur.com/NA8duDO.jpg)  
![](https://i.imgur.com/Sljwq1J.jpg)  


