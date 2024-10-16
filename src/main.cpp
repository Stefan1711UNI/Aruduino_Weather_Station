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
void buttonrequest();
void isRefreshed();
//void buttonpressed();


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
int hum, insideTemp;
int maxTemp, minTemp;
int maxHum, minHum;
int buttonstate = 0;  //button state 
int buttonStatus2 = 0;  
bool refershed = false;

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
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonrequest, FALLING);//interupt routine
  
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
    lcd.print(insideTemp);
    lcd.write(0);
    lcd.print(" ");
    lcd.print(hum);
    lcd.print("%");
    delay(5000);
  }else{    //0 means sensor did not read correctly
    Serial.println("NOT read correctly");
    delay(3000);
  }
}

//Reads the DHT11 sensor
int readDHT(){
  hum = (int) round(dht.readHumidity());   //rads data from sensor
  insideTemp = (int) round(dht.readTemperature());
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
  lcd.print("|");
  lcd.print(maxHum);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Min: ");
  lcd.print(minTemp);
  lcd.write(0);
  lcd.print("|"); 
  lcd.print(minHum);
  lcd.print("%");
  //Other temp sensor
  //code to display out max/min
}

void buttonrequest()
{
  buttonstate++;
  refershed = false;
  if(buttonstate >= 3)
  {
    buttonstate = 0;
  }
  buttonStatus2 = digitalRead(BUTTON);


   static uint32_t previous = millis();

}

// depending on button state different screens are displayed
void display()
{
    buttonStatus2 = digitalRead(BUTTON);


   static uint32_t previous = millis();

  if(buttonstate == 0){   //displayes inside data
    isRefreshed();
    disDHT();
  }else if(buttonstate == 1){   //displays outside data
    isRefreshed();
    LIGHT();
    //also needs temps
  }else if (buttonStatus2 == HIGH ) 
  {
    if (millis() - previous > 2000)
     //switches on the LED after 2 second press and hold
    isRefreshed();
    disOutMax();
  }
  else 
  {
    previous = millis();
  }
}

void isRefreshed(){
  if(refershed == false){
    lcd.clear();
    refershed = true;
  }
}

