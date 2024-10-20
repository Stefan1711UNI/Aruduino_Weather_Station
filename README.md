# Weather Station

Made in C++ for Arduino with Visual Studio Code

Outputs:
- Temperature in Celcius and Humidity % Inside
- Temperature in Celcius Outside
- Indication of Day or Night

## Table of Contents
- Requirements
- Installation
- Configuration
- How it Works

## Requiremants
- [Visual Studio Code with C](https://code.visualstudio.com/download)
- [Platform IO IDE](https://platformio.org/platformio-ide)
- [Arduino (deprecated)](https://github.com/arduino/arduino-cli)
- Arduino UNO R4 WiFi or any compatable version of Arduino
- USB-C cable to connect Arduino to PC
- LM35 Temperature Sensor
- DHT11 Humidity and Temperature Sensor
- LDR Light Sensor
- LCD1602A LCD Display
- Push Button
- 2x 10k Olm Resistors or 1x 5k olm Resistor

### Optional
- Dev Containers
- Docker

## Installation
- Download Zip file from Github
- Extract the contents into Platform IO IDE folder

## Configuration 
- Assemble LM35, DHT11, LDR, and LCD1602A on Breadboard and connect to Arduino Pins accordingly
- Plug in Arduino UNO R4 WiFi or complatable version of Arduino via USB-C cable
- Verify the port and board are correctly connected and chosen
- Compile and upload the code
- Reset Arduino

## How it Works
This program First displays the Message: "Weather Station", and pauses breifly to allow
initilization of the Components. It then Displays the current readings from the Sensors
onto the LCD. This includes Inside Temperature and humidity, Outside Temperature, and 
custom Characters of a Sun and Moon to indicate weather Day or Night is detected.
When the Pushbutton is pressed, it will cause the Display to change to the inside and outside 
Minimum and Maximum Temperatures, that are being stored by the program. When the Pushbutton
is held longer than 1 second, it will Reset the Minimum and Maximum stored. Pressing the Push-
button again will then re-display the first screen.
