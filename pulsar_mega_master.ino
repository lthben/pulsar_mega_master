/*
   Author: Benjamin Low

   Date: August 2016

   Description:

       Code for an Arduino Mega master.

       For a month-long installation at the National Museum of Singapore called 'Into Pulsar'
       by artist Ryf Zaini. This version is different from the earlier Night Fest version
       and does not have LED lights.

       Three bicycles placed at the bottom of a 'tree'. User cycles any of these to trigger the
       28 x ebike wheels on top of the tree to go faster. As the user continues cycling, in a
       span of 20 seconds, 5 more relays will be triggered in sequence to activate the bicycle
       gears and sprockets to make the installation come alive progressively. At the end of 20
       seconds, the flood lights will turn on for about 8 seconds.

       The 28 ebike motor controllers are controlled via 14 PWM pins on the master mega. One pin
       for 2 controllers. Hence there is no client mega.

       PWM pins 2, ..., 13, 38, 44, 45, 46

       Note: Cannot have delay() in program else rpm calculation will be affected
             Comment out all Serial.print() statements to free up processor speed when deployed 
*/

#define hallPin1 22 //hall sensor
#define hallPin2 26
#define hallPin3 30

#define ledPin1 23 //data pin of the led strips
#define ledPin2 24
#define ledPin3 25
#define ledPin4 27

#define ebikePin1 2 //ebike motor controller
#define ebikePin2 3
#define ebikePin3 4
#define ebikePin4 5
#define ebikePin5 6
#define ebikePin6 7
#define ebikePin7 8
#define ebikePin8 9
#define ebikePin9 10
#define ebikePin10 11
#define ebikePin11 12
#define ebikePin12 13
#define ebikePin13 44
#define ebikePin14 45

#define floodRelayPin1 47 //flood lights
#define floodRelayPin2 48

#define motorRelayPin1 49 //dc motors
#define motorRelayPin2 50
#define motorRelayPin3 51
#define motorRelayPin4 52
#define motorRelayPin5 53

#define potPin A0 //for testing only, simulates the maxRPM

#include "FastLED.h"

#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 300//mix of 4m and 5m, 60 leds per m
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

CRGB strip1[NUM_LEDS_PER_STRIP];
CRGB strip2[NUM_LEDS_PER_STRIP];
CRGB strip3[NUM_LEDS_PER_STRIP];
CRGB strip4[NUM_LEDS_PER_STRIP];

void setup() {

  Serial.begin(9600);

  pinMode(hallPin1, INPUT_PULLUP);
  pinMode(hallPin2, INPUT_PULLUP);
  pinMode(hallPin3, INPUT_PULLUP);
  
  pinMode(ebikePin1, OUTPUT);
  pinMode(ebikePin2, OUTPUT);
  pinMode(ebikePin3, OUTPUT);
  pinMode(ebikePin4, OUTPUT);
  pinMode(ebikePin5, OUTPUT);
  pinMode(ebikePin6, OUTPUT);
  pinMode(ebikePin7, OUTPUT);
  pinMode(ebikePin8, OUTPUT);
  pinMode(ebikePin9, OUTPUT);
  pinMode(ebikePin10, OUTPUT);
  pinMode(ebikePin11, OUTPUT);
  pinMode(ebikePin12, OUTPUT);
  pinMode(ebikePin13, OUTPUT);
  pinMode(ebikePin14, OUTPUT);

   pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  pinMode(floodRelayPin1, OUTPUT); 
  pinMode(floodRelayPin2, OUTPUT);
  
  pinMode(motorRelayPin1, OUTPUT);
  pinMode(motorRelayPin2, OUTPUT);
  pinMode(motorRelayPin3, OUTPUT);
  pinMode(motorRelayPin4, OUTPUT);
  pinMode(motorRelayPin5, OUTPUT);

  pinMode(potPin, INPUT);

  digitalWrite(floodRelayPin1, HIGH);//turn off as all relays are active low
  digitalWrite(floodRelayPin2, HIGH);
  digitalWrite(motorRelayPin1, HIGH);
  digitalWrite(motorRelayPin2, HIGH);
  digitalWrite(motorRelayPin3, HIGH);
  digitalWrite(motorRelayPin4, HIGH);
  digitalWrite(motorRelayPin5, HIGH);

  FastLED.addLeds<NEOPIXEL, ledPin1>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, ledPin2>(leds[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, ledPin3>(leds[2], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, ledPin4>(leds[3], NUM_LEDS_PER_STRIP);

  init_LEDs();
}


void loop() {

  //  use_pot(); //for testing simulation only

  calc_RPM(); //from hall sensor

  update_ebike_speed();//ebike

  update_LEDs();

  update_relays();//motors for the gears and sprockets, as well as flood lights
}






