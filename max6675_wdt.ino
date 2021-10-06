#include<avr/wdt.h>
//#include <EEPROM.h>
#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>
#include <SmoothThermocouple.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//pin alarm
int alarm_pin =13;

////pin isnan
//int isnan_pin =12;

//wdt_reset_pin
//int wdt_reset_pin = 11;

//PIN MAX6675 yang dihubungkan dengan arduino UNO
#define SCK_PIN 3
#define CS_PIN 4
#define SO_PIN 5


//millis
const unsigned long eventInterval = 5000;
unsigned long previousTime = 0;

/**
  Smoothing factor of a temperature value.
*/
#define SMOOTHING_FACTOR 5

Thermocouple* thermocouple = NULL;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
   lcd.init();
  lcd.backlight();
  Serial.print ("Arduino reset");
  Serial.println();
  lcd.setCursor(0,0);
  lcd.print ("Arduino reset..");
  delay (2000);

  
  // PIN MODE
pinMode (alarm_pin,OUTPUT);  
//pinMode (isnan_pin,OUTPUT);
//pinMode (wdt_reset_pin,INPUT_PULLUP);
//WDT watchdog timer enable
wdt_enable ( WDTO_8S);

  Thermocouple* originThermocouple = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN);
  thermocouple = new SmoothThermocouple(originThermocouple, SMOOTHING_FACTOR);
}


// the loop function runs over and over again forever
void loop() {

 

////update frequenly
unsigned long currentTime = millis();
//// event coding
if (currentTime - previousTime >= eventInterval) {  
 lcd.clear();

  delay (500);
  
  // Reads temperature
  float celsius = thermocouple->readCelsius();
   float kelvin = thermocouple->readKelvin();
   float fahrenheit = thermocouple->readFahrenheit();
   
   if ((float) celsius > 35.00){
 digitalWrite (alarm_pin, HIGH);
 delay (50);
 digitalWrite (alarm_pin,LOW);
 delay(50);
 Serial.println ("temperature terlalu tinggi");
 
lcd.setCursor(2,0);
lcd.print ("Temperature:");
lcd.setCursor(4,1);
lcd.print (celsius);
lcd.print( "\xDF" "C");
delay (2500);
lcd.setCursor(2,0);
lcd.print ("Suhu terbaca");
lcd.setCursor(2,1);
lcd.print ("tinggi > 35""\xDF""C");

}
else if ((float) celsius < 35.00){
digitalWrite (alarm_pin,LOW);
Serial.println ("temperature Normal");

lcd.setCursor(2,0);
lcd.print ("Temperature:");
lcd.setCursor(4,1);
lcd.print (celsius);
lcd.print( "\xDF" "C");
delay (2500);
lcd.setCursor(2,0);
lcd.print ("Suhu Terbaca");
lcd.setCursor(4,1);
lcd.print ("Normal...");

}
else if (isnan (celsius)){
 Serial.println ("temperature tidak terbaca");
 lcd.setCursor(2,0);
lcd.print ("Temperature:");
lcd.setCursor(4,1);
lcd.print (celsius);
lcd.print( "\xDF" "C");
delay (2500);
lcd.setCursor(3,0);
lcd.print ("Suhu Tidak");
lcd.setCursor(2,1);
lcd.print ("Terbaca...");
void(* resetFunc) (void) = 0;//declare reset function at address 0
 delay(3000);
 resetFunc(); //call reset  
}

// Output of information
  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.println(" C, ");
//  Serial.print(kelvin);
//  Serial.print(" K, ");
//  Serial.print(fahrenheit);
//  Serial.println(" F");
/*//OUTPUT SERIAL JSON
Serial.print("{\"VAR1\":");
 Serial.print(celsius);
 Serial.print(",\"VAR2\":");
 Serial.print(kelvin);
 Serial.print(",\"VAR3\":");
 Serial.print(fahrenheit);
 Serial.println("}");
*/

////LCD Print 
//lcd.clear();
//lcd.setCursor(2,0);
//lcd.print ("Temperature:");
//lcd.setCursor(4,1);
//lcd.print (celsius);
//lcd.print( "\xDF" "C");

previousTime = currentTime;
 }

  
wdt_reset(); 
}
 
