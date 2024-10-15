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



// KAMY
#define LDR A0 // LDR at port A0
int ldr;
int LED = 13; // On-board LED at 13
int RELAY = 2; // RELAY at port 2

LCD_I2C lcd(0x27, 16, 2);

#define dhtPin 8  //DHT sensor on pin 2
DHT dht(dhtPin, DHT11);    

//global variable can be accessed anyware
float hum, insideTemp;
float maxTemp, minTemp;
float maxHum, minHum;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  dht.begin();

  pinMode(LED, OUTPUT); // LED is output
  pinMode(RELAY, OUTPUT); // RELAY is output
  digitalWrite(LED, LOW); // LED OFF at beginning
  digitalWrite(RELAY, LOW); // RELAY OFF at beginning

}

void loop() {
  disDHT();
  LIGHT();
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
    lcd.print(insideTemp);
    lcd.setCursor(0,1);
    lcd.print("Hum=");
    lcd.print(hum);
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

void display(){
  disDHT();
  LIGHT();
  
}