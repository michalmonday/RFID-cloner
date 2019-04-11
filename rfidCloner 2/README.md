
# List of contents  
* [Devices](#devices)  
* [Features](#features)  
* [Bugs](#bugs)  
* [Required libraries](#required-libraries)  
* [Credits](#credits) 


# Devices 

Initially I created a device with NodeMCU and prototyping board. See [this wiki page](https://github.com/michalmonday/RFID-cloner/wiki/NodeMCU-with-prototyping-board-device) for details about it.    
<img src="https://camo.githubusercontent.com/641823241b7dda7315d9dcba9ab2433322982fda/68747470733a2f2f692e696d6775722e636f6d2f4e34426a4a77652e6a7067" width="400" />   

Then I designed a PCB to make it smaller and better looking. See [this wiki page](https://github.com/michalmonday/RFID-cloner/wiki/Custom-PCB-with-Esp-12F-device-(first-version)) for details about it.  
<img src="https://raw.githubusercontent.com/michalmonday/files/master/RFID-Cloner/images%20first%20PCB/front.jpg" width="400" />   

I will try to design one with built-in charging circuit, micro USB port and CH340C chip so it will be possible to program it without any adapters, just like NodeMCU.   

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


# Required libraries  
-[rfid](https://github.com/miguelbalboa/rfid/)  
-[ESP8266 OLED SSD1306](https://github.com/ThingPulse/esp8266-oled-ssd1306/)  
-[LinkedList](https://github.com/ivanseidel/LinkedList)  


# Credits  
The creators of libraries and boards/modules used in this project. RFID functionality is based on the [RFID library](https://github.com/miguelbalboa/rfid/) created by Miguel Balboa and other contributors. When trying to implement menu I was relying on the [esp8266_deauther](https://github.com/spacehuhn/esp8266_deauther/blob/master/esp8266_deauther/DisplayUI.cpp) project created by spacehuhn.
