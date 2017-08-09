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
| VCC | 1k resistor + red LED | GND |

**Batteries:**
I used 3 AA batteries (1.5V each) in "Series" connection to provide 4.5V to the device. Check this [image](http://www.12voltbattery.info/images/content/batteryWireDiagram.jpg) to see the principle on how to connect the batteries.

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

