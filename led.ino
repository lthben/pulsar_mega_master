void init_LEDs() {

  int brightLevel = 32; //max seems to be 32 else will see voltage drop effect

  show_default_white();

  FastLED.setBrightness(brightLevel);
  FastLED.show();
}

void update_LEDs() {

//  have some tolerance to create artificial delay() for the lights
  if (myMaxRPM < myPrevMaxRPM - 5 || myMaxRPM > myPrevMaxRPM + 5) {

    int brightLevel = map(myMaxRPM, MINRPM, MAXRPM, 4, 32);

    FastLED.setBrightness(brightLevel);

    if (myMaxRPM < MAXRPM) {  //default white

      pulsate_white();

    } else {

      rainbowCycle(); //special effect at max speed
    }

    myPrevMaxRPM   = myMaxRPM;
    FastLED.show(); //always show something from the last state
    
  } 
}

int pulseDir = 1;
int myBrightLevel = 0;

void pulsate_white() { 
    myBrightLevel += pulseDir;
    if (myBrightLevel == 0 || myBrightLevel == 32)
        pulseDir = -pulseDir;
        
  for (int x = 0; x < NUM_STRIPS; x++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = CRGB::White;
    }
  }
}

void show_default_white() {
  for (int x = 0; x < NUM_STRIPS; x++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = CRGB::White;
    }
  }
}

// this makes the rainbow equally distributed throughout
void rainbowCycle() {
  uint16_t i, j;

  for (int x = 0; x < NUM_STRIPS; x++) {
    for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
      for (i = 0; i < NUM_LEDS_PER_STRIP; i++) {
        leds[x][i] = CRGB(Wheel(((i * 256 / NUM_LEDS_PER_STRIP) + j) & 255));
      }
    }
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {

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


