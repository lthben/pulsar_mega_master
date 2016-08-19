void update_LEDs() {

  if (myMaxRPM != myPrevMaxRPM) {

    int brightLevel = map(myMaxRPM, MINRPM, MAXRPM, 32, 255);

    FastLED.setBrightness(brightLevel);

    //    for (int x = 0; x < NUM_STRIPS; x++) {
    //      for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    //        leds[x][i] = CRGB::White;
    //      }
    //    }

    for (int x = 0; x < NUM_STRIPS; x++) {

      rainbow(x, 20);
      //    rainbowCycle(x, 20);
      //    theaterChaseRainbow(x, 50);


      FastLED.show();
      //delay(20);//for the default white only

      myPrevMaxRPM = myMaxRPM;
    }
  }
}

void rainbow(int x, uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = CRGB(Wheel((i + j) & 255));
    }
    FastLED.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(int x, uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = CRGB(Wheel(((i * 256 / NUM_LEDS_PER_STRIP) + j) & 255));
    }
    FastLED.show();
    delay(wait);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(int x, uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < NUM_LEDS_PER_STRIP; i = i + 3) {
        leds[x][i+q] = CRGB(Wheel( (i + j) % 255)); //turn every third pixel on
      }
      FastLED.show();

      delay(wait);

      for (uint16_t i = 0; i < NUM_LEDS_PER_STRIP; i = i + 3) {
        leds[x][i+q] = CRGB(0);      //turn every third pixel off
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


