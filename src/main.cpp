#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LCD_I2C.h>
#include <Wire.h>

#define TEMP A1 //LM35 pin

//functions
int readDHT();
void getMaxIns(double insideTemp, double hum);
void disDHT();
void LIGHT();
void LM35read();
void display();
void isRefreshed();
void getMaxOut(double insideTemp);
//button functions
void buttonInterruptHandler();
void incrementCounterOnPress();
void checkLongPress();



//LDR
#define LDR A0 // LDR at port A0
int ldr;
int RELAY = 2; // RELAY at port 2
int BUTTON = 3;
//LCD
LCD_I2C lcd(0x27, 16, 2);
//DHT11
#define dhtPin 8  //DHT sensor on pin 8
DHT dht(dhtPin, DHT11);    
bool buttonpress = true;    

byte sun1[] =
{
  B00000,
  B01010,
  B00000,
  B10011,
  B00010,
  B00100,
  B10100,
  B00100,
};
byte sun2[] =
{
  B00000,
  B01010,
  B00000,
  B11001,
  B01000,
  B00100,
  B00101,
  B00100,
};
byte sun3[] =
{
  B00100,
  B10100,
  B00100,
  B00010,
  B10011,
  B00000,
  B01010,
  B00000,
};
byte sun4[] =
{
  B00100,
  B00101,
  B00100,
  B01000,
  B11001,
  B00000,
  B01010,
  B00000,
};
byte moon1[] =
{
  B00000,
  B00000,
  B00011,
  B00100,
  B00111,
  B00100,
  B00000,
  B00000,
};
byte moon2[] =
{
  B00000,
  B00000,
  B11000,
  B00100,
  B10010,
  B01001,
  B00101,
  B00101,
};
byte moon3[] =
{
  B00000,
  B00000,
  B00100,
  B00111,
  B00100,
  B00011,
  B00000,
  B00000,
};
byte moon4[] =
{
  B00101,
  B00101,
  B01001,
  B10010,
  B00100,
  B11000,
  B00000,
  B00000,
};

//global variable
int hum, insideTemp, outsideTemp;
int maxTempOut, minTempOut, maxTempIns, minTempIns;
int maxHum, minHum;
int buttonstate = 0;  //button state   
//if screen has been cleared
bool refershed = false;

//For button to determin if held
const int buttonPin = 3;    // Pin number for the button
int buttonCounter = 0;      // Global variable to track button presses
bool isLongPressed = false; // Global variable to track long press
unsigned long pressedTime = 0;  // Time when the button was pressed
unsigned long releasedTime = 0; // Time when the button was released

void setup() 
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  dht.begin();
  //Switch to Internal 1.1V Reference
  analogReference(AR_INTERNAL);

  //LDR
  pinMode(RELAY, OUTPUT); // RELAY is output
  digitalWrite(RELAY, LOW); // RELAY OFF at beginning
  pinMode(BUTTON, INPUT_PULLUP); // BUTTON input

  //Sun/Moon characters
  lcd.createChar(0, sun1);
  lcd.createChar(1, sun2);
  lcd.createChar(2, sun3);
  lcd.createChar(3, sun4);
  lcd.createChar(4, moon1);
  lcd.createChar(5, moon2);
  lcd.createChar(6, moon3);
  lcd.createChar(7, moon4);
  //button
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterruptHandler, CHANGE);
}

void loop() {
  display();
}

//gets and sets max and min values of DHT11 sensor
void getMaxIns(double insideTemp, double hum){
  if(insideTemp>maxTempIns){
    maxTempIns = insideTemp;
  }else if(insideTemp<minTempIns){
    minTempIns = insideTemp;
  }
  if(hum>maxHum){
    maxHum = hum;
  }else if(hum<minHum){
    minHum = hum;
  }
}

//gets and sets max and min values of LM35 sensor
void getMaxOut(double outsideTemp){
  if(outsideTemp>maxTempOut){
    maxTempOut = outsideTemp;
  }else if(outsideTemp<minTempOut){
    minTempOut = outsideTemp;
  }
}

//Resets max and min values
void resetMax(){
  maxTempIns = 0;
  minTempIns = 0;
  maxHum = 0;
  minHum = 0;
  maxTempOut = 0;
  minTempOut = 0;
}

//displays DHT11 sensor data
void disDHT(){
  if(readDHT()==1){   //1 means sensor read correctly
    lcd.setCursor(0,0);
    lcd.print("Inside:");
    lcd.print(insideTemp, 1);
    lcd.print((char)223);
    lcd.setCursor(0,1);
    lcd.print(hum);
    lcd.print("%");
    //delay(5000);
  }else{    //0 means sensor did not read correctly
    Serial.println("NOT read correctly");
    delay(3000);
  }
}

//Reads the DHT11 sensor
int readDHT(){
  hum = dht.readHumidity();   //rads data from sensor
  insideTemp = dht.readTemperature();
  if(isnan(hum) || isnan(insideTemp)){  //isnan checks if is not a number(error code for dht is not a number)
    return 0;
  }else {
    getMaxIns(insideTemp, hum);    //updates max/min values
    getMaxOut(outsideTemp);
    return 1;
  }
}

void LIGHT()
{
  ldr = analogRead(LDR); // Read temperature
  Serial.print("ldr value:\n");
  Serial.println(ldr);
  if(ldr > 800) // If dark detected
  {
    digitalWrite(RELAY, HIGH); // RELAY ON
    lcd.setCursor(13,0);
    lcd.write(0);// sun
    lcd.write(1);
    lcd.setCursor(13, 1);
    lcd.write(2);
    lcd.write(3);
  }
  else
  {
    digitalWrite(RELAY, LOW); // RELAY OFF
    lcd.setCursor(13, 0);
    lcd.write(4); //moon
    lcd.write(5);
    lcd.setCursor(13, 1);
    lcd.write(6);
    lcd.write(7);
  }
}

//screen to display max/min values out+in
void disMax(){
  //DHT11 data
  lcd.setCursor(0,0);
  lcd.print("Max:");
  lcd.print(maxTempIns, 1);
  lcd.print((char)223); 
  lcd.print(maxHum);
  lcd.print("%|");
  //LM35 data
  lcd.setCursor(10,0);
  lcd.print(maxTempOut, 1);
  lcd.print((char)223); 
  //DHT11 data
  lcd.setCursor(0,1);
  lcd.print("Min:");
  lcd.print(minTempIns, 1);
  lcd.print((char)223); 
  lcd.print(minHum);
  lcd.print("%|");
  //LM35 data
  lcd.setCursor(10,1);
  lcd.print(minTempOut, 1);
  lcd.print((char)223); 

  /* Debugging output for Serial monitor
  Serial.print("Max Inside Temp: "); Serial.println(maxTempIns);
  Serial.print("Min Inside Temp: "); Serial.println(minTempIns);
  Serial.print("Max Humidity: "); Serial.println(maxHum);
  Serial.print("Min Humidity: "); Serial.println(minHum);
  Serial.print("Max Outside Temp: "); Serial.println(maxTempOut);
  Serial.print("Min Outside Temp: "); Serial.println(minTempOut);*/
}

// depending on button state different screens are displayed
void display()
{
  //long press resets max/min values
  if (isLongPressed) {  //checks if the button was pressed for long
    isLongPressed = false; // Reset long press status for next detection
    isRefreshed();    
    resetMax();
  }else if(buttonCounter == 0){   //displayes inside data
    isRefreshed();
    LIGHT();
    disDHT();
  }else if(buttonCounter == 1){   //displays outside data
    isRefreshed();
    LIGHT();
    LM35read();
  }else{  //displays max values
    isRefreshed();
    disMax();
  }
}

void LM35read() 
{
  // read the input on analog pin 0:
  outsideTemp = analogRead(TEMP);
  //Calculate Temperature from TEMP value
  //Note that we use mV for Vref
  //Vin = TEMPresult*Vref/(2^10)
  //Temp(C) = Vin/(10) = TEMPresult*Vref/(1200*10) + 2 
  outsideTemp = outsideTemp*1100/(1200*10.0) + 2.0;
  lcd.setCursor(0,0);
  lcd.print("Outside:");
  lcd.print(outsideTemp, 1);
  lcd.print((char)223);
  lcd.print("C");
  getMaxOut(outsideTemp);
  delay(2500);
}

//checks if screen has been cleared, to prevent flickering
void isRefreshed(){
  if(refershed == false){
    lcd.clear();
    refershed = true;
  }
}

//checks if button was pressed
void buttonInterruptHandler() {
  // Use this function to handle button press/release
  if (digitalRead(buttonPin) == LOW) { // Button pressed
    pressedTime = millis();
  } else { // Button released
    releasedTime = millis();
    checkLongPress();
    incrementCounterOnPress();
    
  }
}

//changes button counter 
void incrementCounterOnPress() {
  buttonCounter++;
  refershed = false;
  if(isLongPressed == true){  //to go to screen max/min
    buttonCounter = 2;
  }
  if (buttonCounter >= 3) {
    buttonCounter = 0; // Reset counter to 0 if it reaches or exceeds 3
  }
}

// Function to check if the button has been long pressed
void checkLongPress() {
  unsigned long heldTime = releasedTime - pressedTime;
  if (heldTime >= 1000) { 
    isLongPressed = true;
  }
}