float pulseBrightLevel;
int pulseDir = 1;
float wipeColorIndex;

long maxSpeedTriggeredTime, //when the flood lights are triggered
     isFirstMaxSpeedTime, //when max speed is first triggered;
     isFloodLightsOffTime; //time when flood lights turn off
bool hasMaxSpeedTriggered, //whether user has triggered max speed RPM
     hasFirstMaxSpeedTriggered,
     isFloodLightOn;

//light modes that vary with MaxRPM
const int PULSATE = 0;
const int RAINBOWCYCLE = 1;
const int VUMETER = 3; //volume meter light effect with buildup over time

int num_leds_per_strip; //for VUMETER mode
const int MAXRPM = 150; //max speed of use bicycle needed to trigger max speed for pov
const int MINRPM = 20; //min speed below which will read as an off signal

//user settings
const int whichLEDMode = VUMETER;

const int TEMPDURATION = 5000; //duration in ms the flood lights is turned on temporarily
const int PLAYDURATION = 15000;//duration for play mode before the flood lights can be triggered
const int IGNOREDURATION = 15000; //duration after flood lights turn off to ignore rpm readings to let the wheels slow down cos of fluctuating rpm readings and will become less than MAXRPM

const int MAXBRIGHTLEVEL = 128; //before visible voltage drop occurs
CRGB pulseColor = CRGB(MAXBRIGHTLEVEL, MAXBRIGHTLEVEL, MAXBRIGHTLEVEL); //the colour for the pulse mode

void init_LEDs() {

  turn_off_leds();
}

void update_LEDs() {

  if (millis() - maxSpeedTriggeredTime > TEMPDURATION && millis() - isFloodLightsOffTime > IGNOREDURATION ) {
    //only after the flood lights have turned on for TEMPDURATION and given time for speed to slow down inbetween user rounds

    if (myRawMaxRPM == MAXRPM) {

      if (!hasFirstMaxSpeedTriggered) { //need MAXRPM to trigger the timer for PLAYDURATION

        hasFirstMaxSpeedTriggered = true;
        isFirstMaxSpeedTime = millis();

        Serial.println("first max speed triggered");

      } else { //first max speed has triggered

        if (millis() - isFirstMaxSpeedTime > PLAYDURATION) { //user has played for more then PLAYDURATION

//          if (whichLEDMode == VUMETER) {

//            if (num_leds_per_strip == NUM_LEDS_PER_STRIP) {

//              trigger_installation();
//            }

//          } else { //other LED modes are less strict

            trigger_installation();

//          }
        } else { //less than PLAYDURATION

          play_with_leds_normally();
        }
      }

    } else { //less than MAXRPM

      if (!hasFirstMaxSpeedTriggered && whichLEDMode == VUMETER) { //dark until user does something

        pulsate(pulseColor);

      } else {

        hasMaxSpeedTriggered = false;

        play_with_leds_normally();

//        if (millis() - isFirstMaxSpeedTime > PLAYDURATION) { //idling so reset
//
//          hasFirstMaxSpeedTriggered = false;
//
//        } else {
//
//          play_with_leds_normally();
//        }
      }
    }
  }
}

void trigger_installation() {

  hasMaxSpeedTriggered = true; //this will trigger the flood lights

  maxSpeedTriggeredTime = millis();

  Serial.println();
  Serial.println("hasMaxSpeedTriggered triggered");
  Serial.println();
}

void play_with_leds_normally() {

  if (whichLEDMode == PULSATE) {

    pulsate(pulseColor);

  } else if (whichLEDMode == RAINBOWCYCLE) {

    rainbowCycle();

  } else if (whichLEDMode == VUMETER) {

    vumeter();
  }
}

void pulsate(CRGB _theColor) {

  //  float pulseSpeed = map(myRawMaxRPM, MINRPM, MAXRPM, 2, 50); //set the pulse speed range here, integer values only
  float pulseSpeed = 3;
  pulseBrightLevel += (pulseDir * pulseSpeed); //can also set the speed here

  if (pulseBrightLevel <= 0) {
    pulseBrightLevel = 0;
    pulseDir = -pulseDir;
  }
  if ( pulseBrightLevel >= MAXBRIGHTLEVEL) {
    pulseBrightLevel = MAXBRIGHTLEVEL;
    pulseDir = -pulseDir;
  }

  for (int x = 0; x < NUM_STRIPS; x++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = _theColor;
    }
  }

  FastLED.setBrightness(int(pulseBrightLevel));
  FastLED.show();
}

void rainbowCycle() {

  float wipeSpeed = map(myRawMaxRPM , MINRPM, MAXRPM, 1, 60); //set the colorwipe speed range here, integer values only
  wipeColorIndex += wipeSpeed / 2.0; //can also set the speed here
  int myColorIndex = int(wipeColorIndex) % 255;

  //  Serial.print("wipeColorIndex: ");
  //  Serial.print(wipeColorIndex);
  //  Serial.println();

  for (int x = 0; x < NUM_STRIPS; x++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = Wheel(myColorIndex);
    }
  }
  FastLED.show();
}

void vumeter() {

  if (hasFirstMaxSpeedTriggered) {

    float wipeSpeed = map(myRawMaxRPM , MINRPM, MAXRPM, 1, 60); //set the colorwipe speed range here, integer values only
    wipeColorIndex += wipeSpeed / 2.0; //can also set the speed here
    int myColorIndex = int(wipeColorIndex) % 255;

    calc_num_leds_per_strip();

    for (int x = 0; x < NUM_STRIPS; x++) {
      for (int i = 0; i < num_leds_per_strip; i++) {
        leds[x][i] = Wheel(myColorIndex);
      }
      for (int j = NUM_LEDS_PER_STRIP - 1; j >= num_leds_per_strip; j--) {
        leds[x][j] = CRGB(0, 0, 0);
      }
    }

    FastLED.show();

  } else { //first max speed not triggered yet
    turn_off_leds();
  }
}

void calc_num_leds_per_strip() { //VUMETER responds in real time to cyclists effort

  num_leds_per_strip = int ( ( (millis() - isFirstMaxSpeedTime) * 1.0 / PLAYDURATION ) * NUM_LEDS_PER_STRIP);

//      if (myRawMaxRPM > MINRPM + 10) { //as long as one of the bicycles is going somewhat fast
//        num_leds_per_strip++;
//      } else {
//        num_leds_per_strip--;
//      }

  num_leds_per_strip = constrain(num_leds_per_strip, 0, NUM_LEDS_PER_STRIP);

//    Serial.print("num leds per strip: ");
//    Serial.print(num_leds_per_strip);
//    Serial.print("    ");
//    Serial.print("rawMaxRPM: ");
//    Serial.println(myRawMaxRPM);
}

void turn_off_leds() {
  for (int x = 0; x < NUM_STRIPS; x++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = CRGB(0, 0, 0);
    }
  }
  FastLED.show();
}

void show_default_white() {
  for (int x = 0; x < NUM_STRIPS; x++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = CRGB(MAXBRIGHTLEVEL, MAXBRIGHTLEVEL, MAXBRIGHTLEVEL);
    }
  }
  FastLED.show();
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(int WheelPos) {

  WheelPos = 255 - WheelPos;

  if (WheelPos < 85) {
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }

  WheelPos -= 170;

  return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void update_flood_lights() {

  if (hasMaxSpeedTriggered == true) {

    if (millis() - maxSpeedTriggeredTime > 500) { //flood lights takes about a sec to turn on

      turn_off_leds();
    }
    if (isFloodLightOn == false) {

      digitalWrite(relayPin1, LOW); //relay is active low. Turn on all flood lights.
      digitalWrite(relayPin2, LOW);
      digitalWrite(relayPin3, LOW);
      digitalWrite(relayPin4, LOW);
      digitalWrite(relayPin5, LOW);
      digitalWrite(relayPin6, LOW);

      isFloodLightOn = true;

      Serial.println();
      Serial.println("    flood light on    ");
      Serial.println();
    }
  }

  if (hasMaxSpeedTriggered == false && isFloodLightOn == true) {

    digitalWrite(relayPin1, HIGH); //turn off
    digitalWrite(relayPin2, HIGH);
    digitalWrite(relayPin3, HIGH);
    digitalWrite(relayPin4, HIGH);
    digitalWrite(relayPin5, HIGH);
    digitalWrite(relayPin6, HIGH);

    isFloodLightOn = false;

    hasFirstMaxSpeedTriggered = false;
    wipeColorIndex = 0.0; //prevent buffer overflow
    isFloodLightsOffTime = millis();

    turn_off_leds();

    Serial.println();
    Serial.println("    flood light off    ");
    Serial.println();
  }
}


