![](https://i.imgur.com/N4BjJwe.jpg)  

[Youtube demo video link](https://www.youtube.com/watch?v=K7hS6TLmSes)  

# List of contents  
* [Features](#features)  
* [Bugs](#bugs)  
* [Parts used](#parts-used)  
* [Required libraries](#required-libraries)  
* [NodeMCU settings](#nodemcu-settings)  
* [Wiring](#wiring)  
* [Additional information](#additional-information)  
* [Future](#future)  
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
-NodeMCU (version with CP2102) (e.g. [ebay link](https://www.ebay.co.uk/itm/NodeMcu-ESP8266-ESP-12E-CH340-WIFI-4MB-CP2102-Lua-USB-Arduino-Raspberry-PI-IoT/263103158702))  
-RC522 RFID reader module (e.g. [ebay link](https://www.ebay.co.uk/itm/RFID-RC522-Mifare-Card-reader-Arduino-or-raspberry-pi-UK-Stock/161862594146))  
-SSD1306 OLED 1.3" display (i2C - 4 pin version) (e.g. [ebay link](https://www.ebay.co.uk/itm/1-3-128x64-IIC-I2C-SPI-OLED-Display-Module-for-Arduino-White-Blue-/282358769160))  
-MIFARE 1K RFID card with changeable UID - 13.56Mhz (e.g. [ebay link](https://www.ebay.co.uk/itm/5PCS-UID-card-Changeable-with-Phone-0-Sector-0-block-Rewritable-M1-IC-card-UK/272720001067))  
-some RFID card to copy (only 13.56Mhz cards can be read/written by this particular card reader module)  
-7 buttons  (e.g. [ebay link](https://www.ebay.co.uk/itm/100Pcs-6-6-5-mm-4Pin-Tactile-Touch-Push-Button-Switch-Tact-Switches-6-X-6-X-5mm/191737340275))  
-resistors (10k, 4.7k, 2.2k, 2x1k, 680, 470, 220) (e.g. [ebay link](https://www.ebay.co.uk/itm/Resistors-Metal-Film-300-Pack-10-each-30-values-1-4w-1-Kit-Assortment-Mix-UK/111148362636))  
-prototyping board (e.g. [ebay link](https://www.ebay.co.uk/itm/5Pcs-Double-Side-PCB-Prototype-Circuit-Printed-Soldring-Track-Strip-Board-5x7cm/262317473034))  
-cables (e.g. [ebay link](https://www.ebay.co.uk/itm/30AWG-Insulated-Silver-Plated-Single-Core-Copper-PCB-0-25mm-Kynar-Wrapping-Wire/263504549866))  
-switch  (e.g. [ebay link](https://www.ebay.co.uk/itm/40pcs-3-Terminals-ON-ON-2-Positions-SPDT-Electronic-Push-Button-Sliding-Switches/183144329954))  
-soldering equipment (soldering iron, tin, holder)  
-hot glue (or tape)  
-3.7V li-po battery with charger ([ebay link](https://www.aliexpress.com/item/4pcs-3-7V-JJRC-H31-400mAh-3-7V-30C-Lipo-Battery-and-4in1-Battery-charger-box/32831397411.html)) (or just 3 AAA batteries)  
-2 pin dupont connector (for easier battery replacement/removal). I used [these](https://www.ebay.co.uk/itm/Connector-Plug-Male-Wire-560pcs-Awg-Jumper-Pin-Header-18-26-Kit-Crimp-Dupont/292431010145)    
>See Asian ebay/aliexpress listings for cheaper alternatives.  
   


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
| D5 | SPI SCK |
| D6 | SPI MISO |
| D7 | SPI MOSI |
| D8 | SPI SS |
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

Hand-drawn diagram: [Link](https://i.imgur.com/01dBMua.png)  

Fritzing diagram:  

The order of resistors between buttons matters.  The order of 3V3 (red wire) between buttons does not matter, as long as each button (and 10K resistor) is connected to 3V3 it will be good.  

![diagram image](https://i.imgur.com/a5x5Ri7.png)


# Additional information   
Always make sure to **switch off** the battery before connecting device through USB to PC.  
There is no step-by-step tutorial yet, you could contact me on discord (michalmonday#3687) if you'd like to build it but aren't sure about something.  

# Future  
I am planning to make a PCB that will be used instead of the prototyping board. The idea is to make the device as big in lenght and width as RC522 module. I hope that with 3x30x40mm battery it will look much better. It should be also easier to assemble because there will be no need to cut wires, there will be some small SMD components but it shouldn't be a problem using normal soldering iron with pointed tip, tweezers and preferably 0.3mm solder. It will require FT232RL serial adapter to flash Esp-12F. The current state of design can be seen here:  
https://easyeda.com/michalmonday17/rfid-cloner-without-cp2102

It wasn't tested yet, I'm waiting for all the parts to arrive, I'll probably write a small guide if it will work.  

I think that the PCB would be much better if it had CP2102 chip so it could be programmed through micro USB just like NodeMcu, but it would require hot air station and I am too afraid that I will mess something up in design. Once I get the "non-CP2102" version to work, I'll probably try to make one with CP2102.  


# Credits  
The creators of libraries and boards/modules used in this project. RFID functionality is based on the [RFID library](https://github.com/miguelbalboa/rfid/) created by Miguel Balboa and other contributors. When trying to implement menu I was relying on the [esp8266_deauther](https://github.com/spacehuhn/esp8266_deauther/blob/master/esp8266_deauther/DisplayUI.cpp) project created by spacehuhn.

# Pictures  
**Most recent pictures**  
![](https://i.imgur.com/IgH1A0L.png)  
![](https://i.imgur.com/5ZSVX3p.png)  
![](https://i.imgur.com/b0DXIjV.png)  
![](https://i.imgur.com/Qw5NgJn.png)  
![](https://i.imgur.com/4g8NkTs.png)  

**Previous pictures (using 3xAAA batteries)**  
(It didn't have 10k resistor yet, in case if someone used these images for wiring reference.)  
![](https://i.imgur.com/2CDCzr6.jpg)  
![](https://i.imgur.com/7RNlH2X.jpg)  
![](https://i.imgur.com/NA8duDO.jpg)  
![](https://i.imgur.com/Sljwq1J.jpg)  
