#include <Servo.h>
  Servo servo1, servo2;


/*****SERVO Variable******/
#define MAX_POSITION 140
#define MIN_POSITION 60
#define SERVO_LATANCY 5
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

uint8_t cnt = 0;
void setup() {
  Serial.begin(9600);
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(LDR3, INPUT);
  pinMode(LDR4, INPUT);

  while(1){
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

  Init_LDR();


  

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
  }delay(1000);

  for(int i = MAX_POSITION; i > MIN_POSITION; i--){
    servoMoving(i);
    delay(SERVO_LATANCY);
  }delay(1000);
  
}



void Init_LDR(){
  int LDR_TL = analogRead(LDR4); // Top Left
  int LDR_TR = analogRead(LDR3); // Top Right
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
}
