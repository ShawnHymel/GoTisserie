/**
 * XBee RC Controller
 * Author: Shawn Hymel
 * Date: January 22, 2014
 *
 * Sends thumb joystick ADC over an XBee. Back to front is -100
 * to 100. Right to left is -100 to 100. Designed for a SparkFun
 * FIO v3.
 *
 * Hardware Connections:
 *    FIO      Joystick Breakout
 *    --------------------------
 *    GND      GND
 *    3V3      VCC
 *    A3       VERT
 *    A4       HORZ
 */

#include <SoftwareSerial.h>

// To calibrate the joystick:
//  - Change DEBUG to 1
//  - Run program and open Serial Monitor
//  - Do not touch joystick and record horiz/vert values
//  - Fill in constants below for horiz/vert centers
//  - Change DEBUG back to 0
//  - Re-compile and upload
#define DEBUG 0
#define VERTICAL_CENTER 507
#define HORIZONTAL_CENTER 516

// If you change the default XBee baud, update it here
#define XBEE_BAUD 9600

// Number of milliseconds between transmission bursts
#define XMIT_DELAY 20

// Pin connections
const int VERTICAL_PIN = 3;
const int HORIZONTAL_PIN = 4;
const int XBEE_RX = 0;
const int XBEE_TX = 1;

// Global variables
int vertical;
int horizontal;
int8_t remapped_vertical;
int8_t remapped_horizontal;
SoftwareSerial xbee(XBEE_RX, XBEE_TX);

void setup() {

  // If debugging, initialize serial monitor
#if DEBUG == 1
  Serial.begin(9600);
#endif

  // Initialize serial connection to XBee
  xbee.begin(XBEE_BAUD);

}

void loop() {
  
  // Read values from joystick
  vertical = analogRead(VERTICAL_PIN);
  horizontal = analogRead(HORIZONTAL_PIN);
  
  // If debugging, print out values to serial monitor
#if DEBUG == 1
  Serial.print("Vertical: ");
  Serial.print(vertical, DEC);
  Serial.print(" | Horizontal: ");
  Serial.println(horizontal, DEC);
#endif 

  // Re-map vertical values to account for off-center
  if ( vertical == VERTICAL_CENTER ) {
    vertical = 512;
  } else if ( vertical < VERTICAL_CENTER ) {
    vertical = map(vertical, 0, VERTICAL_CENTER - 1, 0, 511);
  } else if ( vertical > VERTICAL_CENTER ) {
    vertical = map(vertical, VERTICAL_CENTER + 1, 1023, 513, 1023);
  }  
  
  // Re-map horizontal values to account for off-center
  if ( horizontal == HORIZONTAL_CENTER ) {
    horizontal = 512;
  } else if ( horizontal < HORIZONTAL_CENTER ) {
    horizontal = map(horizontal, 0, HORIZONTAL_CENTER - 1, 0, 511);
  } else if ( horizontal > HORIZONTAL_CENTER ) {
    horizontal = map(horizontal, HORIZONTAL_CENTER + 1, 1023, 513, 1023);
  }  

  // Re-map joystick values from [0 1023] to [-100 100]
  remapped_vertical = map(vertical, 0, 1023, -100, 100);
  remapped_horizontal = map(horizontal, 0, 1023, -100, 100);
  
  // Transmit values over XBee
  xbee.write(remapped_vertical);
  xbee.write(remapped_horizontal);
  
  // Wait 20ms between transmission bursts
  delay(XMIT_DELAY);
}
