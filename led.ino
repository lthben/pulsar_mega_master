float pulseBrightLevel;
int pulseDir = 1;
float wipeColorIndex;

const int PULSATE = 0;
const int COLORWIPE = 1;

long isMaxSpeedTime; //when max speed is triggered;
bool isFloodLightOn;

//user settings
int whichLEDMode = COLORWIPE;
const int MAXBRIGHTLEVEL = 128; //before visible voltage drop occurs
CRGB pulseColor = CRGB(MAXBRIGHTLEVEL, MAXBRIGHTLEVEL, MAXBRIGHTLEVEL); //the colour for the pulse mode 
const int TEMPDURATION = 5000; //duration in ms the flood lights is turned on temporarily

void update_LEDs() {

  if (millis() - isMaxSpeedTime > TEMPDURATION) {

    if (myRawMaxRPM > MAXRPM - 10) {

      turn_off_leds();

      isMaxSpeed = true;

      isMaxSpeedTime = millis();

      Serial.println();
      Serial.println("isMaxSpeed triggered");
      Serial.println();

    } else { //below max speed

      isMaxSpeed = false;

      if (whichLEDMode == PULSATE) {

        pulsate(pulseColor);

      } else if (whichLEDMode == COLORWIPE) {

        rainbowCycle();
      }
    }
  }
}

void pulsate(CRGB _theColor) {

  float pulseSpeed = map(myRawMaxRPM, MINRPM, MAXRPM, 1, 20); //set the pulse speed range here, integer values only
  pulseBrightLevel += (pulseDir * pulseSpeed) / 10.0; //can also set the speed here

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

  float wipeSpeed = map(myRawMaxRPM , MINRPM, MAXRPM, 1, 20); //set the colorwipe speed range here, integer values only
  wipeColorIndex += wipeSpeed / 10.0; //can also set the speed here
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
      leds[x][i] = CRGB::White;
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

  if (isMaxSpeed == true && isFloodLightOn == false) {

    digitalWrite(relayPin1, LOW); //relay is active low. Turn on all flood lights.
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin3, LOW);
    digitalWrite(relayPin4, LOW);
    digitalWrite(relayPin5, LOW);
    digitalWrite(relayPin6, LOW);

    isFloodLightOn = true;

    //      Serial.println();
    //      Serial.println("    flood light on    ");
    //      Serial.println();
  }

  if (isMaxSpeed == false && isFloodLightOn == true) {

    digitalWrite(relayPin1, HIGH); //turn off
    digitalWrite(relayPin2, HIGH);
    digitalWrite(relayPin3, HIGH);
    digitalWrite(relayPin4, HIGH);
    digitalWrite(relayPin5, HIGH);
    digitalWrite(relayPin6, HIGH);

    isFloodLightOn = false;

    //      Serial.println();
    //      Serial.println("    flood light off    ");
    //      Serial.println();
  }
}


