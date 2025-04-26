#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

/********** User variables *************/
float setTemp = 38.0;     // Pump ON temperature

/********** Pin Definitions *************/
#define ONE_WIRE_BUS 2    // DS18B20 pin
#define LDR1 A0           // Top Right
#define LDR2 A1           // Bottom Left
#define LDR3 A2           // Bottom Right
#define LDR4 A3           // Top Left
#define PUMP_PIN 8        // Pump relay pin

/********** Servo Variables *************/
#define MAX_POSITION 140
#define MIN_POSITION 60
int servo_1_position = 90;   // Up/Down Servo
int servo_2_position = 90;   // Left/Right Servo

Servo servo1;   // Up/Down
Servo servo2;   // Left/Right

/********** Sensor Variables *************/
int LDR_TL, LDR_TR, LDR_BL, LDR_BR;
float tempC;

// Libraries Setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  // Set LDR pins
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(LDR3, INPUT);
  pinMode(LDR4, INPUT);

  // Pump output
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);  // Initially OFF

  // LCD Setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sun Tracker Boot");
  delay(2000);
  lcd.clear();

  // Temperature sensor
  sensors.begin();

  // Servo Setup
  servo1.attach(9);   // Servo 1 to pin 9
  servo2.attach(10);  // Servo 2 to pin 10
  servo1.write(servo_1_position);
  servo2.write(servo_2_position);

  delay(1000);
}

void loop() {
  readTemperature();
  readLDRs();
  adjustServos();

  delay(100); // smooth operation
}

/********** Functions *************/

// Read LDR values
void readLDRs() {
  LDR_TL = analogRead(LDR4); // Top Left
  LDR_TR = analogRead(LDR1); // Top Right
  LDR_BL = analogRead(LDR2); // Bottom Left
  LDR_BR = analogRead(LDR3); // Bottom Right

  Serial.print("TL: "); Serial.print(LDR_TL);
  Serial.print(" | TR: "); Serial.print(LDR_TR);
  Serial.print(" | BL: "); Serial.print(LDR_BL);
  Serial.print(" | BR: "); Serial.println(LDR_BR);
}

// Read temperature and control pump
void readTemperature() {
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print((char)223);  // Degree Symbol
  lcd.print("C    ");

  if (tempC >= setTemp) {
    digitalWrite(PUMP_PIN, HIGH); // Turn Pump ON
    lcd.setCursor(0, 1);
    lcd.print("Pump: ON ");
  } else if (tempC <= (setTemp - 4)) {
    digitalWrite(PUMP_PIN, LOW); // Turn Pump OFF
    lcd.setCursor(0, 1);
    lcd.print("Pump: OFF");
  }
}

// Adjust servo positions based on LDR
void adjustServos() {
  int top_average = (LDR_TL + LDR_TR) / 2;
  int bottom_average = (LDR_BL + LDR_BR) / 2;
  int left_average = (LDR_TL + LDR_BL) / 2;
  int right_average = (LDR_TR + LDR_BR) / 2;

  int vertical_diff = top_average - bottom_average;    // Up/Down
  int horizontal_diff = left_average - right_average;  // Left/Right

  int threshold = 50; // minimum difference to move

  // Vertical Movement
  if (abs(vertical_diff) > threshold) {
    if (vertical_diff > 0) {
      servo_1_position--;   // Move Up
    } else {
      servo_1_position++;   // Move Down
    }
  }

  // Horizontal Movement
  if (abs(horizontal_diff) > threshold) {
    if (horizontal_diff > 0) {
      servo_2_position--;   // Move Left
    } else {
      servo_2_position++;   // Move Right
    }
  }

  // Constrain positions
  servo_1_position = constrain(servo_1_position, MIN_POSITION, MAX_POSITION);
  servo_2_position = constrain(servo_2_position, MIN_POSITION, MAX_POSITION);

  // Move servos
  //servo1.write(servo_1_position);
  servo2.write(servo_2_position);

  Serial.print("Servo1 (Up/Down): ");
  Serial.print(servo_1_position);
  Serial.print(" | Servo2 (Left/Right): ");
  Serial.println(servo_2_position);
}
