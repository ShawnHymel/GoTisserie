/**
 * Trike_Motor_Driver
 *
 * Controls the motor and servo for a single-wheel drive
 * tricycle using an XBee radio for a remote control.
 *
 * Hardware Connections:
 *
 * Arduino    Hardware
 * ----------------------
 *   5        Servo input
 *   6        PWMA (TB6612FNG)
 *   8        AIN1 (TB6612FNG)
 *   9        AIN2 (TB6612FNG)
 *   10       STBY (TB6612FNG)
 */
 
#include <Servo.h>
#include <SoftwareSerial.h>

// If you change the default XBee baud, update it here
#define XBEE_BAUD 9600

// Pins
const int XBEE_RX = 0;
const int XBEE_TX = 1;
const int SERVO_PIN = 5;
const int PWMA = 6;
const int AIN1 = 8;
const int AIN2 = 9;
const int STBY = 10;

// Global variables
Servo servo;
int pos = 0;
int state = 0;
SoftwareSerial xbee(XBEE_RX, XBEE_TX);

// Setup
void setup() {
  
  // ***TEST
  Serial.begin(9600);
  
  // Initialize XBee
  xbee.begin(XBEE_BAUD);
  
  // Set pin modes
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  
  // Initialize servo
  servo.attach(SERVO_PIN);
}

// Main loop
void loop() {
  
  // Determine position for servo
  switch (state) {
  case 0:
    pos = 1;
    state = 1;
    break;
  case 1:
    pos = 90;
    state = 2;
    break;
  case 2:
    pos = 179;
    state = 3;
    break;
  case 3:
    pos = 90;
    state = 0;
    break;
  default:
    break;
  }
  
  // Set servo at desired position
  servo.write(pos);
  
  // Motor full speed, forward
  move(255, 1);
  delay(1000);
  stop();
  delay(250);
  
  // Motor half speed, backward
  move(255, 0);
  delay(1000);
  stop();
  delay(250);
}

// Move motor A at a speed and direction
void move(int spd, int dir) {
  
  boolean inPin1;
  boolean inPin2;
  
  // Disable standby
  digitalWrite(STBY, HIGH);
  
  // Set direction
  if ( dir == 1 ) {
    inPin1 = HIGH;
    inPin2 = LOW;
  } else {
    inPin1 = LOW;
    inPin2 = HIGH;
  }
  
  // Set speed
  digitalWrite(AIN1, inPin1);
  digitalWrite(AIN2, inPin2);
  analogWrite(PWMA, spd);
}

// Stops all motors by putting the motor driver in standby mode
void stop() {
  digitalWrite(STBY, LOW);
}
