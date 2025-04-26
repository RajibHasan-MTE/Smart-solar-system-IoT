#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


/**********User variable*************/
float setTemp = 38.0;

// Pin where the DS18B20 is connected
#define ONE_WIRE_BUS 2

#include <Servo.h>
  Servo servo1, servo2;


/*****SERVO Variable******/
#define MAX_POSITION 140
#define MIN_POSITION 60
#define SERVO_LATANCY 50
int servo_1_position = 0;
int servo_2_position = 0;
int init_position = 90;
int servo_1_pos = 90;
int servo_2_pos = 90;

/***********LDR_Pin define***********/
#define LDR1 A0
#define LDR2 A1
#define LDR3 A2
#define LDR4 A3

int LDR_TL;
int LDR_TR;
int LDR_BL;
int LDR_BR;

uint8_t cnt = 0;

#define PUMP_PIN 8
float tempC;
// Setup a oneWire instance
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire to DallasTemperature library
DallasTemperature sensors(&oneWire);

// Set the LCD address and size
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(LDR3, INPUT);
  pinMode(LDR4, INPUT);

  lcd.init();           // Initialize the LCD
  lcd.backlight();      // Turn on the backlight

  sensors.begin();      // Start the DS18B20 sensor

  lcd.setCursor(0, 0);
  lcd.print("Temp Sensor");
  delay(1000);
  lcd.clear();

  while(0){
    int LDR_TL = analogRead(LDR4); // Bottom Right
     Serial.println(LDR_TL);
     delay(100);
  }

  servo1.attach(9);   // Attach servo to pin 9
  servo2.attach(10);
  servoMoving(120);

  delay(100);

}

void loop() {
  Init_temp();
  Init_LDR();
  //servo_test();

  if(LDR_TR > LDR_BL){
    servo_test();
  }


  

cnt++;
delay(10);
}

void servoMoving(int pos) {

  int servo_1_pos = pos;
  int servo_2_pos = 180 - pos;

  servo1.write(servo_1_pos);
  servo2.write(servo_2_pos);


  /*
    Serial.print("Servo-1: ");
  Serial.print(servo_1_pos);
  Serial.print(" | ");
  Serial.print("Servo-2: ");
  Serial.print(servo_2_pos);
  Serial.print(" | ");
  Serial.println();
  */
  
}

void servo_test(){

  for(int i = MIN_POSITION; i < MAX_POSITION; i++){
    servoMoving(i);
    delay(SERVO_LATANCY);
  }delay(100);

  for(int i = MAX_POSITION; i > MIN_POSITION; i--){
    servoMoving(i);
    delay(SERVO_LATANCY);
  }delay(100);
  
}



void Init_LDR(){
  LDR_TL = analogRead(LDR4); // Top Left
  LDR_TR = analogRead(LDR3); // Top Right
  LDR_BL = analogRead(LDR1); // Bottom Left
  LDR_BR = analogRead(LDR2); // Bottom Right

  // Print with labels
  Serial.print("TL: ");
  Serial.print(LDR_TL);

  Serial.print(" | TR: ");
  Serial.print(LDR_TR);

  Serial.print(" | BL: ");
  Serial.print(LDR_BL);

  Serial.print(" | BR: ");
  Serial.println(LDR_BR);
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
