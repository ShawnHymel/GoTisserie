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
 *   Rx       XBee Rx
 *   Tx       XBee Tx
 *   5        Servo input
 *   6        PWMA (TB6612FNG)
 *   8        AIN1 (TB6612FNG)
 *   9        AIN2 (TB6612FNG)
 *   10       STBY (TB6612FNG)
 */
 
#include <Servo.h>

/***************************************************************
 * Globals
 **************************************************************/
// Useful for debugging on the Arduino Mega 2560
#define DEBUG 1

// If you change the default XBee baud, update it here
#define XBEE_BAUD 9600

// Number of milliseconds between tranmission bursts
#define XMIT_DELAY 20

// Pins
const int SERVO_PIN = 5;
const int PWMA = 6;
const int AIN1 = 8;
const int AIN2 = 9;
const int STBY = 10;

// Global variables
int state = 0;
int8_t vertical = 0;
int8_t horizontal = 0;
int8_t pos;
unsigned long start_time;
Servo servo;

/***************************************************************
 * Setup
 **************************************************************/
void setup() {
  
  // Initialize alternate serial for debugging
#if DEBUG == 1
  Serial1.begin(9600);
#endif
  
  // Initialize XBee
  Serial.begin(XBEE_BAUD);
  
  // Set pin modes
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  
  // Initialize servo
  servo.attach(SERVO_PIN);
  servo.write(90);
  
}

/***************************************************************
 * Main Loop
 **************************************************************/
void loop() {
  
  // Wait to get 2 bytes from XBee
  start_time = millis();
  while ( (Serial.available() < 2) && 
          ((millis() - start_time) < XMIT_DELAY) ) {
  }
  
  // Flush buffer if less than 2 bytes received, otherwise read
  if ( Serial.available() < 2 ) {
    Serial.flush();
  } else {
    vertical = Serial.read();
    horizontal = Serial.read();
    Serial.flush();
  }
  
  // Set motor speed
  
  //Re-map horizontal values to servo position and set servo
  pos = map(horizontal, -100, 100, 1, 179);
  servo.write(pos);
  
  // Print received values to alternate serial for debugging
#if DEBUG == 1
  Serial1.print("V: ");
  Serial1.print(vertical, DEC);
  Serial1.print(" | POS: ");
  Serial1.println(pos, DEC);
#endif
  
}

/****************************************************************
 * Functions
 ***************************************************************/
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
