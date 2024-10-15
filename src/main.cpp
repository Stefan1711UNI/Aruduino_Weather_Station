#include <Arduino.h>
#include "DHT.h"
#include "LCD_I2C.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LCD_I2C.h>

//functions
int readDHT();
void getMax(double insideTemp, double hum);
void disDHT();
void LIGHT();
void display();


// KAMY
#define LDR A0 // LDR at port A0
int ldr;
int LED = 13; // On-board LED at 13
int RELAY = 2; // RELAY at port 2
int PED = 0;//pedestrian button status
int BUTTON = 3;

LCD_I2C lcd(0x27, 16, 2);

#define dhtPin 8  //DHT sensor on pin 8
DHT dht(dhtPin, DHT11);    
bool buttonpress = true;    

byte degree[] = {   //custom character for degree celcius
  B11011,
  B11100,
  B00100,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000
};

//global variable can be accessed anyware
float hum, insideTemp;
float maxTemp, minTemp;
float maxHum, minHum;
int buttonPress = 0;    //button state

void setup() 
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  dht.begin();

  pinMode(LED, OUTPUT); // LED is output
  pinMode(RELAY, OUTPUT); // RELAY is output
  digitalWrite(LED, LOW); // LED OFF at beginning
  digitalWrite(RELAY, LOW); // RELAY OFF at beginning
  pinMode(BUTTON, INPUT_PULLUP); // BUTTON input
  
  lcd.createChar(0, degree);
}

void loop() {
  display();
}

//gets and sets max and min values of DHT11 sensor
void getMax(double insideTemp, double hum){
  if(insideTemp>maxTemp){
    maxTemp = insideTemp;
  }else if(insideTemp<minTemp){
    minTemp = insideTemp;
  }
  if(hum>maxHum){
    maxHum = hum;
  }else if(hum<minHum){
    minHum = hum;
  }
}

//displays DHT11 sensor data
void disDHT(){
  delay(2000);
  if(readDHT()==1){   //1 means sensor read correctly
    lcd.setCursor(0,0);
    lcd.print((int) round(insideTemp));
    lcd.write(0);
    lcd.print(" ");
    lcd.print((int) round(hum));
    lcd.print("%");
    delay(5000);
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
    getMax(insideTemp, hum);    //updates max/min values
    return 1;
  }
}

void LIGHT()
{
  ldr = analogRead(LDR); // Read temperature
  if(ldr > 800) // If dark detected
  {
  digitalWrite(LED, HIGH); // LED ON
  digitalWrite(RELAY, HIGH); // RELAY ON
  lcd.setCursor(12,0);
  lcd.print("Night");
  }
  else
  {
  digitalWrite(LED, LOW); // LED OFF
  digitalWrite(RELAY, LOW); // RELAY OFF
  lcd.setCursor(13,0);
  lcd.print("Day");
  }
}

//screen to display max/min values out+in
void disOutMax(){
  //DHT11 data
  lcd.setCursor(0,0);
  lcd.print("Max: ");
  lcd.print(maxTemp);
  lcd.write(0); 
  lcd.print(maxHum);
  lcd.print("|");
  lcd.setCursor(0,1);
  lcd.print("Min: ");
  lcd.print(minTemp);
  lcd.write(0); 
  lcd.print(minHum);
  lcd.print("|");
  //Other temp sensor
  //code to display out max/min
}

//depending on button state different screens are displayed
void display(){
  if(buttonPress == 0){   //displayes inside data
    disDHT();
  }else if(buttonPress == 1){   //displays outside data
    LIGHT();
    //also needs temps
  }else{  //displays max values
    //display max values
  }
}

//button code
void buttonpressed()
{
  static int lastbuttonstate = HIGH;
  int read = digitalRead(BUTTON);
  if (read != lastbuttonstate)
  {
    delay(50);
    if(read == LOW)
    {
      buttonpress = !buttonpress;
      delay(200);
    }
  }
  lastbuttonstate = read;

  if(buttonpress)
  {
    disDHT();
  }
  else
  {
    LIGHT();
  }
}