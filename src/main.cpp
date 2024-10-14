//STEFAN SONDERLING
//DHT11 SENSOR WILL NOT WORK ON VSCODE AS THE LIBARY DOES NOT EXIST. PLUG BACK INTO ARDUINO IDE ENVIROMENT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DHT.h"
#include "LCD_I2C.h"

LCD_I2C lcd(0x27, 16, 2);

#define dhtPin 2  //DHT sensor on pin 2
DHT dht(dhtPin, DHT11);    

//global variable can be accessed anyware
float hum, insideTemp;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  dht.begin();

}

void loop() {
  disDHT();

}

void disDHT(){
  delay(2000);
  if(readDHT()==1){
    lcd.setCursor(0,0);
    lcd.print("Temp=");
    lcd.print(insideTemp);
    lcd.setCursor(0,1);
    lcd.print("Humidity=");
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
    return 1;
  }

}