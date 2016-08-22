int pulseBrightLevel;
int pulseDir = 1;
float wipeSpeed;
float wipeColorIndex;

const int PULSATE = 0;
const int COLORWIPE = 1;

//user settings
int whichMode = PULSATE;
const int MAXBRIGHTLEVEL = 128; //before visible voltage drop occurs
CRGB pulseColor = CRGB(MAXBRIGHTLEVEL, MAXBRIGHTLEVEL, MAXBRIGHTLEVEL);

void init_LEDs() {

  pulseBrightLevel = MAXBRIGHTLEVEL / 4;
}

void update_LEDs() {

  if (whichMode == PULSATE) {

    pulsate(pulseColor);

  } else if (whichMode == COLORWIPE) {

    rainbowCycle();
  }

  if (myRawMaxRPM == MAXRPM) {

    turn_off_leds();

    isMaxSpeed = true;
  }
}

void pulsate(CRGB _theColor) {

  float pulseSpeed = map(myRawMaxRPM, MINRPM, MAXRPM, 1, 32);
  pulseBrightLevel += (pulseDir * pulseSpeed);
  
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

  FastLED.setBrightness(pulseBrightLevel);
  FastLED.show();
}

void rainbowCycle() {    

  wipeSpeed = map(myRawMaxRPM , MINRPM, MAXRPM, 5, 200);
  wipeColorIndex += wipeSpeed / 10.0;
  int myColorIndex = int(wipeColorIndex) % 255;
  
  Serial.print("wipeColorIndex: ");
  Serial.print(wipeColorIndex);
  Serial.println();
    
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
//    Serial.println("return 0");
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
//    Serial.println("return 1");
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }

  WheelPos -= 170;

//  Serial.println("return 2");
  return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void update_flood_lights() {

  if (isMaxSpeed == true) {

    digitalWrite(relayPin1, LOW); //turn on all flood lights
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin3, LOW);
    digitalWrite(relayPin4, LOW);
    digitalWrite(relayPin5, LOW);
    digitalWrite(relayPin6, LOW);

    delay(5000); //stop everything

    isMaxSpeed = false;

    digitalWrite(relayPin1, HIGH); //turn off
    digitalWrite(relayPin2, HIGH);
    digitalWrite(relayPin3, HIGH);
    digitalWrite(relayPin4, HIGH);
    digitalWrite(relayPin5, HIGH);
    digitalWrite(relayPin6, HIGH);
  }
}


