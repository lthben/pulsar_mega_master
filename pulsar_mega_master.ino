/*
   Author: Benjamin Low

   Date: August 2016

   Description:

       Code for an Arduino Mega master.

       For a Night Festival 2016 installation at the National Museum of Singapore
       called 'Into Pulsar' by artist Ryf Zaini.

       Three bicycles placed at the bottom of a 'tree'. User cycles any of these to trigger the
       28 x ebike wheels on top of the tree to go faster and 4 x 4m led strips to react in
       brightness and color.

       The master mega reads the three hall sensor values and and sends it to the slave
       via pwm output to an analog input pin on the slave mega. The master also controls
       the 4 strips of LEDs. The master and slave mega each only has 15 pwn pins
       to control 14 motor controllers each. The slave just controls 14 motor controllers.

       PWM pins 2, ..., 13, 38, 39, 40
*/
#include "FastLED.h"

#define hallPin1 22
#define hallPin2 26
#define hallPin3 30

#define motorPin1 2
#define motorPin2 3
#define motorPin3 4
#define motorPin4 5
#define motorPin5 6
#define motorPin6 7
#define motorPin7 8
#define motorPin8 9
#define motorPin9 10
#define motorPin10 11
#define motorPin11 12
#define motorPin12 13
#define motorPin13 38
#define motorPin14 39

#define motorOutPin 40 //to the mega slave

#define ledPin1 44
#define ledPin2 46
#define ledPin3 48
#define ledPin4 50

#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 240 //4m, 60 leds per m
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

CRGB strip1[NUM_LEDS_PER_STRIP];
CRGB strip2[NUM_LEDS_PER_STRIP];
CRGB strip3[NUM_LEDS_PER_STRIP];
CRGB strip4[NUM_LEDS_PER_STRIP];

int currVal[3], prevVal[3], counter[3];
int RPM[3], myMaxRPM, myPrevMaxRPM, myRawMaxRPM;
long isTriggeredTime[3], prevReadTime[3];
long oneRevTimeInterval[3], timeInterval[3];

#define MAXRPM 100 //max speed of use bicycle needed to trigger max speed for pov
#define MINRPM 20 //min speed below which will read as an off signal

void setup() {

  Serial.begin(9600);

  pinMode(hallPin1, INPUT_PULLUP);
  pinMode(hallPin2, INPUT_PULLUP);
  pinMode(hallPin3, INPUT_PULLUP);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin6, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);
  pinMode(motorPin9, OUTPUT);
  pinMode(motorPin10, OUTPUT);
  pinMode(motorPin11, OUTPUT);
  pinMode(motorPin12, OUTPUT);
  pinMode(motorPin13, OUTPUT);
  pinMode(motorPin14, OUTPUT);
  pinMode(motorOutPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

 FastLED.addLeds<NEOPIXEL, ledPin1>(leds[0], NUM_LEDS_PER_STRIP);
 FastLED.addLeds<NEOPIXEL, ledPin2>(leds[1], NUM_LEDS_PER_STRIP);
 FastLED.addLeds<NEOPIXEL, ledPin3>(leds[2], NUM_LEDS_PER_STRIP);
 FastLED.addLeds<NEOPIXEL, ledPin4>(leds[3], NUM_LEDS_PER_STRIP);
}

void loop() {

  for (int i = 0; i < 3; i++) {
    read_hall_sensor(i);
    calc_RPM(i);
  }

  get_myMaxRPM();

  control_motor_speed();

  update_LEDs();
}


