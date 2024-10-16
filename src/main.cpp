#include <Arduino.h>
#include "DHT.h"
#include "LCD_I2C.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LCD_I2C.h>
#include <Wire.h>
#define TEMP A1

//functions
int readDHT();
void getMax(double insideTemp, double hum);
void disDHT();
void LIGHT();
void LM35read();

// KAMY
#define LDR A0 // LDR at port A0
int ldr;
int LED = 13; // On-board LED at 13
int RELAY = 2; // RELAY at port 2
int PED = 0;//pedestrian button status
int BUTTON = 3;

LCD_I2C lcd(0x27, 16, 2);

#define dhtPin 8  //DHT sensor on pin 2
DHT dht(dhtPin, DHT11);
bool buttonpress = true;    

byte degree[] = {
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

void setup() 
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  dht.begin();
    //Switch to Internal 1.1V Reference
  analogReference(AR_INTERNAL);

  pinMode(LED, OUTPUT); // LED is output
  pinMode(RELAY, OUTPUT); // RELAY is output
  digitalWrite(LED, LOW); // LED OFF at beginning
  digitalWrite(RELAY, LOW); // RELAY OFF at beginning
  pinMode(BUTTON, INPUT_PULLUP); // BUTTON input
  
  lcd.createChar(0, degree);
}

void loop() 
{
  buttonpressed();
}

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

void disDHT(){
  delay(2000);
  if(readDHT()==1){
    lcd.setCursor(0,0);
    lcd.print("Temp=");
    lcd.print((int) round(insideTemp));
    lcd.write(0);
    lcd.setCursor(0,1);
    lcd.print("Hum=");
    lcd.print((int) round(hum));
    lcd.print("%");
    delay(5000);
  }else{
    Serial.println("NOT read correctly");
    delay(3000);
  }
}

//Reads the DHT11 sensor
int readDHT(){
  hum = dht.readHumidity();
  insideTemp = dht.readTemperature();

  if(isnan(hum) || isnan(insideTemp)){  //isnan checks if is not a number(error code for dht is not a number)
    return 0;
  }else {
    getMax(insideTemp, hum);
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
  lcd.setCursor(12,0);
  lcd.print("Day");
  }
}

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

void LM35read() 
{
  // read the input on analog pin 0:
  float temperature = analogRead(TEMP);

  //Calculate Temperature from TEMP value
  //Note that we use mV for Vref
  //Vin = TEMPresult*Vref/(2^10)
  //Temp(C) = Vin/(10) = TEMPresult*Vref/(1024*10)
  temperature = temperature*1100/(1023*10);
  Serial.println("Temperature : ");
  Serial.println(temperature);
  lcd.setCursor(0,0);
  lcd.print("Outside:");
  lcd.print(temperature);
  lcd.print((char)223);
  lcd.print("C");
  delay(1500);
}