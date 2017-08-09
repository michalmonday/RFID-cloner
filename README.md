**Parts used:**  
-Arduino Pro Micro 5V 
> (other Arduino also could be used but with different pinout, see the comments at the top of examples included in [this library](https://github.com/miguelbalboa/rfid/))

-MFRC522 RFID reader module  
-button  
-LEDs (red, yellow, green) + resistors (1k ohm)  
-3 AA batteries (1.5V each)  
-switch  
-cables, hot glue, tape  


**Usage of the device:**  
After turning it on the red LED should be on which just indicates that the device is working. After pressing the button once the yellow light should be on, it indicates that the device is waiting for the card. After the card is read the yellow LED shoud be off. Now we can press the button again and all 3 LEDs should be on, it indicates that the device is waiting for the card, but this time it will write to it instead of reading (green LED = writing).

**Wiring:**  

| Arduino Pro Micro | MFRC522 |
| --- | --- |
| RST | RST |
| 10 | SPI SS |
| 16 | SPI MOSI |
| 14 | SPI MISO |
| 15 | SPI SCK |
| GND | GND |
| VCC | VCC (through 2 zener diodes 3.7V) |

| Arduino Pro Micro | Parts | 4.5V Battery (3xAA) |
| --- | --- | --- |
| RAW | SWITCH | V+ |
| GND | - | V- |

| Arduino Pro Micro | Parts | Arduino Pro Micro |
| --- | --- | --- |
| 9 | Button | GND |
| 4 | 1k resistor + yellow LED | GND |
| 5 | 1k resistor + green LED | GND |
| RAW | 1k resistor + red LED | GND |

**Batteries:**
I used 3 AA batteries (1.5V each) in "Series" connection to provide 4.5V to the device. Check this [image](http://www.12voltbattery.info/images/content/batteryWireDiagram.jpg) to see the principle on how to connect the batteries.

**DIY guide:**  
1. Hot glue the Arduino to the back of the card reader module.   
> Try to keep the USB socket easily accessible. Arduino shouldn't be too close to the pins of the card reader because it may be difficult to solder.   
2. Solder all the connections between the Arduino and the card reader module.  
> Remember to put zener diodes between the VCC pin of Arduino and VCC of MFRC522, other pins require simple wires. The direction of zener diodes is important, they should be pointed from the Arduino to MFRC522 (not the other way).  
3. Solder all the other components as shown in the wiring tables above and pictures below.  
4. Connect the device through the micro USB socket to the PC.  
5. Open [Arduino IDE](https://www.arduino.cc/en/Main/Software). 
6. Download [rfid library](https://github.com/miguelbalboa/rfid/), unzip it and put "rfid-master" folder inside your Arduino library folder.
> That's what it looks like in my PC after copying it: C:/Program Files (86x)/Arduino/libraries/rdfid-master  
7. Click on Tools -> Board -> Arduino Leonardo **(Important - incorrect setting of the board may require [reflashing](http://forum.arduino.cc/index.php?topic=376079.0) the arduino or performing a [tricky reset](https://www.youtube.com/watch?v=dFQHXm1y5Io).** 
8. Click on "Tools -> Port -> COM X".  
> The following few steps will test whether the device actually copies RFID chips. It is not necessary, at this point everything is ready to use and the [the main sketch](https://github.com/michalmonday/RFID-cloner/blob/master/RFID-Cloner_withUID/RFID-Cloner_withUID.ino) could be uploaded, however I'd recommend to follow the steps and check whether it works, it's very easy to make a wiring mistake and in such case the device may not work as expected.
9. Click on "File -> Examples -> MFRC522 -> DumpInfo" and upload that sketch by pressing upload button in the top left corner of the Arduino IDE.
10. Click on "Tools -> Serial Monitor", put a MIFARE blank card near the card reader to dump its conent. It should be displayed on the serial monitor. Copy all the data and save it in some txt file which will be used to check whether the data was changed.
11. Paste and upload [the main sketch](https://github.com/michalmonday/RFID-cloner/blob/master/RFID-Cloner_withUID/RFID-Cloner_withUID.ino) to the device.
> Now we'll try to actually copy some card.
12. Press the button once. (Yellow LED should turn on)
13. Put some other card close to the reader and wait till the yellow LED gets turned off.
14. Press the button again. (All the LEDs should be turned on including green one)
15. Put the blank card close to the reader and wait till the green and yellow LEDs are turned off. (The same card that we used to dump data from)
16. Upload the "DumpInfo" example again, open serial monitor, put the card the formerly blank card near the reader, open the txt file saved before and see whether the data has changed.
17. If the data has changed upload [the main sketch](https://github.com/michalmonday/RFID-cloner/blob/master/RFID-Cloner_withUID/RFID-Cloner_withUID.ino) being sure that the device works well.


**Additional info:**  
If you're going to tape the device then it's a good idea to avoid taping the micro USB socket. This way you can upload other sketches to the Arduino at any time. (e.g. to dump the card data in serial monitor)  
Not every card can be rewritten, look for MIFARE cards with changeable UID. (e.g. [ebay link](https://www.ebay.co.uk/p/X-1-UID-Card-Changeable-With-Phone-0-Sector-0-Block-Rewritable-M1-IC-Card-UK/))

**Credits:**  
This project is based on the [RFID library created by Miguel Balboa and other contributors](https://github.com/miguelbalboa/rfid/). I just merged 2 of examples from it into 1 (read/write + UID change) and added the button/LEDs/batteries/workflow.

![](http://i.imgur.com/2quL1nI.jpg)

![](http://i.imgur.com/0yQ4qv6.jpg)

![](http://i.imgur.com/dUufcMI.jpg)

![](http://i.imgur.com/UOkyO0h.jpg)

![](http://i.imgur.com/Wjc0GS5.jpg)

![](http://i.imgur.com/BFMD5n7.jpg)

