![](https://i.imgur.com/N4BjJwe.jpg)  

[Youtube demo video link](https://www.youtube.com/watch?v=K7hS6TLmSes)  

# List of contents  
* [Features](#features)  
* [Bugs](#bugs)  
* [Parts used](#parts-used)  
* [Required libraries](#required-libraries)  
* [NodeMCU settings](#nodemcu-settings)  
* [Wiring](#wiring)  
* [Additional information, safety and contact](#additional-information,-safety-and-contact)  
* [Credits](#credits) 
* [Pictures](#pictures)  


# Features  
-reading / writing cards  
-autosave of last card read to %Last_read% file (to prevent losing it if battery goes down before saving to file)  
-saving / loading card data using internal memory  
-file management (rename/delete/overwrite)  
-device lock, button combination unlocking  
>Main menu -> Settings -> Lock -> Toggle  
>Main menu -> Settings -> Lock -> Set combination  
  

 **Additional user interface features**  
 -indicator in the bottom-right corner showing whether any rfid data is ready for writing (empty/card/some file)  
 -debugging of A0 (analog pin) input for buttons calibration   
 -display of board/memory/sketch/chip/sdk related data  
 >Main menu -> Settings -> Debugging -> Buttons calibration view  
 >Main menu -> Settings -> Debugging -> Info  
 
 # Bugs  
Please let me know if you encounter any  

 

# Parts used 
-NodeMCU (version with CP2102)  
-RC522 RFID reader module  
-SSD1306 OLED display (i2C - 4 pin version)  
-MIFARE 1K RFID card (with changeable UID, 13.56Mhz)  
-some RFID card to copy (only 13.56Mhz cards can be read/written by this particular card reader module)  
-buttons (7 of them)  
-resistors (10k, 4.7k, 2.2k, 2x1k, 680, 470, 220)  
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

# NodeMCU settings  
![](https://i.imgur.com/UzOtdIi.png)

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
  
![](https://i.imgur.com/01dBMua.png)


# Additional information, safety and contact   
Always make sure to **switch off** the battery before connecting device through USB to PC.  
There is no step-by-step tutorial yet, you could contact me on discord (michalmonday#3687) if you'd like to build it but aren't sure about something.  

# Credits  
The creators of libraries and boards/modules used in this project. RFID functionality is based on the [RFID library](https://github.com/miguelbalboa/rfid/) created by Miguel Balboa and other contributors. When trying to implement menu I was relying on the [esp8266_deauther](https://github.com/spacehuhn/esp8266_deauther/blob/master/esp8266_deauther/DisplayUI.cpp) project created by spacehuhn.

# Pictures  
(It didn't have 10k resistor yet, in case if someone used these images for wiring reference.)    
  
![](https://i.imgur.com/2CDCzr6.jpg)  
![](https://i.imgur.com/7RNlH2X.jpg)  
![](https://i.imgur.com/NA8duDO.jpg)  
![](https://i.imgur.com/Sljwq1J.jpg)  


