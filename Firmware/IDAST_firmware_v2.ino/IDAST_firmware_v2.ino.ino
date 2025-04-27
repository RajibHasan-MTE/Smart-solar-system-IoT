#include <Servo.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Define servo objects
Servo servoLeft;
Servo servoRight;

// Define servo pins
#define SERVO_LEFT_PIN 9
#define SERVO_RIGHT_PIN 10
#define ONE_WIRE_BUS 2    // DS18B20 pin
#define PUMP_PIN 8        // Pump relay pin
// Define LDR pins
#define LDR_LEFT A0
#define LDR_RIGHT A1

// Variables
int ldrLeftValue = 0;
int ldrRightValue = 0;

int servoPosition = 90;  // Start from middle (90 degrees)

// Settings
const int threshold = 30;      // Minimum difference to move
const int moveStep = 1;        // How much to move each time
const int minAngle = 30;        // Minimum angle
const int maxAngle = 160;      // Maximum angle

/********** User variables *************/
float setTemp = 38.0;     // Pump ON temperature
float tempC;

// Libraries Setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  // LCD Setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IDAST Project v1");
  delay(2000);
  lcd.clear();
  // Temperature sensor
  sensors.begin();
  // Attach servos
  servoLeft.attach(SERVO_LEFT_PIN);
  servoRight.attach(SERVO_RIGHT_PIN);

  // Set initial position
  servoLeft.write(servoPosition);
  servoRight.write(180 - servoPosition);  // Opposite movement

  // LDR pins
  pinMode(LDR_LEFT, INPUT);
  pinMode(LDR_RIGHT, INPUT);
}

void loop() {
  readTemperature();
  // Read LDR values
  ldrLeftValue = analogRead(LDR_RIGHT);
  ldrRightValue = analogRead(LDR_LEFT);

  // Print values
  Serial.print("LDR Left: ");
  Serial.print(ldrLeftValue);
  Serial.print(" | LDR Right: ");
  Serial.println(ldrRightValue);

  // Compare LDR values
  int diff = ldrLeftValue - ldrRightValue;

  if (abs(diff) > threshold) {
    if (diff > 0) {
      // Sun more to left - move servos left
      servoPosition = servoPosition + moveStep;
    } else {
      // Sun more to right - move servos right
      servoPosition = servoPosition - moveStep;
    }
  }

  // Constrain servo position
  servoPosition = constrain(servoPosition, minAngle, maxAngle);

  // Move servos
  servoLeft.write(servoPosition);
  servoRight.write(180 - servoPosition);  // Reverse for right servo

  delay(100); // Smooth movement
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
