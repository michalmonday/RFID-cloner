![](https://i.imgur.com/Sljwq1J.jpg)

[Youtube demo video link](https://www.youtube.com/watch?v=YDx9BJGscPM)  

# List of contents
* [Parts used](#parts-used)  
* [Required libraries](#required-libraries)  
* [Wiring](#wiring)  
* [Credits](#credits) 


# Parts used 
-NodeMCU (version with CP2102)  
-RC522 RFID reader module  
-MIFARE 1K RFID card (with changeable UID, 13.56Mhz)  
-some RFID card to copy (only 13.56Mhz cards can be read/written by this particular card reader module)  
-buttons (6 of them)  
-resistors (4.7k, 2.2k, 2x1k, 680, 470, 220)  
-cables  
(I'm planning to mount all of these on prototyping board and add small, flat, li-po 3.7V battery and a switch when the parts arrive.)  


# Required libraries  
-[rfid](https://github.com/miguelbalboa/rfid/)  
-[ESP8266 OLED SSD1306](https://github.com/ThingPulse/esp8266-oled-ssd1306/)  
-[LinkedList](https://github.com/ivanseidel/LinkedList)  


# Wiring

| Esp8266 | RFID-RC522 |
| --- | --- |
| D3 | RST |
| D5 | SPI SS |
| D6 | SPI MOSI |
| D7 | SPI MISO |
| D8 | SPI SCK |
| GND | GND |
| 3.3V | 3.3V |

| Esp8266 | SSD1306 128x64 OLED display |
| --- | --- |
| D1 | SDA |
| D2 | SCL |
| 3.3V | VCC |
| GND | GND |

![](https://i.imgur.com/ScR9LEq.png)


# Credits  
The creators of libraries and boards/modules used in this project. RFID functionality is based on the [RFID library](https://github.com/miguelbalboa/rfid/) created by Miguel Balboa and other contributors. When trying to implement menu I was relying on the [esp8266_deauther](https://github.com/spacehuhn/esp8266_deauther/blob/master/esp8266_deauther/DisplayUI.cpp) project created by spacehuhn.


