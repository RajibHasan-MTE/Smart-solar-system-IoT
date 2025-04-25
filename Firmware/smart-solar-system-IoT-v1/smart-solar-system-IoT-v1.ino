#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


/**********User variable*************/
float setTemp = 37.0;

// Pin where the DS18B20 is connected
#define ONE_WIRE_BUS 2

/***********LDR_Pin define***********/
#define LDR1 A0
#define LDR2 A1
#define LDR3 A2
#define LDR4 A3

#define MOTOR_PIN_1 6
#define MOTOR_PIN_2 5

#define PUMP_PIN 8
float tempC;
// Setup a oneWire instance
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire to DallasTemperature library
DallasTemperature sensors(&oneWire);

// Set the LCD address and size
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);

  // while(1){
  //   forward();
  //   delay(1000);
  //   stop();
  //   delay(1000);
  //   reverse();
  //   delay(1000);

  // }
  //  while(1){
  //   digitalWrite(PUMP_PIN, HIGH);
  //   delay(1000);
  //   digitalWrite(PUMP_PIN, !HIGH);
  //   delay(1000);
  // //   forward();
  // //   delay(1000);
  // //   stop();
  // //   delay(1000);
  // //   reverse();
  // //   delay(1000);
  //  }
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(LDR3, INPUT);
  pinMode(LDR4, INPUT);
  delay(100);
  Serial.begin(9600);
  lcd.init();           // Initialize the LCD
  lcd.backlight();      // Turn on the backlight

  sensors.begin();      // Start the DS18B20 sensor

  lcd.setCursor(0, 0);
  lcd.print("Temp Sensor");
  delay(1000);
  lcd.clear();
}

void loop() {
  
  Init_temp();

  int LDR_TL = analogRead(LDR4); // Top Left
  int LDR_TR = analogRead(LDR1); // Top Right
  int LDR_BL = analogRead(LDR3); // Bottom Left
  int LDR_BR = analogRead(LDR2); // Bottom Right

  // Print with labels
  Serial.print("TL: ");
  Serial.print(LDR_TL);
  Serial.print(" | TR: ");
  Serial.print(LDR_TR);
  Serial.print(" | BL: ");
  Serial.print(LDR_BL);
  Serial.print(" | BR: ");
  Serial.println(LDR_BR);

  

  delay(1000);
}


void Init_temp(){

  sensors.requestTemperatures();               // Ask for temperature
  tempC = sensors.getTempCByIndex(0);    // Get temperature in Celsius
  if(tempC >= setTemp){
    digitalWrite(PUMP_PIN, 1);
    lcd.setCursor(1, 0);
    lcd.print("Pump ON ");
  }else{
    digitalWrite(PUMP_PIN, 0);
    lcd.setCursor(1, 1);
    lcd.print("Pump OFF");
  }
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print((char)223);  // Degree symbol
  lcd.print("C");
}

void forward(){
  digitalWrite(MOTOR_PIN_1, 1);
  digitalWrite(MOTOR_PIN_2, 0);
}

void reverse(){
  digitalWrite(MOTOR_PIN_1, 0);
  digitalWrite(MOTOR_PIN_2, 1);
}

void stop(){
  digitalWrite(MOTOR_PIN_1, 0);
  digitalWrite(MOTOR_PIN_2, 0);
}
