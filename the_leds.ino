float pulseBrightLevel;
int pulseDir = 1;
int num_leds_per_strip; //for vumeter mode

const int MAXBRIGHTLEVEL = 128; //before visible voltage drop occurs
CRGB pulseColor = CRGB(MAXBRIGHTLEVEL, MAXBRIGHTLEVEL, MAXBRIGHTLEVEL); //the colour for the pulse mode
CRGB tiffanyBlueColor = CRGB(6, MAXBRIGHTLEVEL, MAXBRIGHTLEVEL-40 );
float pulseSpeed = 3;

void init_LEDs() {
  for (int x = 0; x < NUM_STRIPS; x++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = pulseColor;
    }
  }
  FastLED.setBrightness(0);
  FastLED.show();
}

void update_LEDs() {
  if (!isPlaying && !isCoolingDown) {
    if (!hasInitialised) {
      pulsate();
    } else {
      pulseBrightLevel = MAXBRIGHTLEVEL;
      FastLED.setBrightness(int(pulseBrightLevel));
      vumeter();
    }
  }
}

void vumeter() {
  switch (activationCounter) {
    case (0):
      num_leds_per_strip = 0;
      break;
    case (1):
      num_leds_per_strip = 0 * (NUM_LEDS_PER_STRIP / 5);
      break;
    case (2):
      num_leds_per_strip = 1 * (NUM_LEDS_PER_STRIP / 5);
      break;
    case (3):
      num_leds_per_strip = 2 * (NUM_LEDS_PER_STRIP / 5);
      break;
    case (4):
      num_leds_per_strip = 3 * (NUM_LEDS_PER_STRIP / 5);
      break;
    case (5):
      num_leds_per_strip = NUM_LEDS_PER_STRIP;
      break;
    case (6):
      num_leds_per_strip = 0;
      break;
    default:
      break;
  }
  show_vumeter();
}

void show_vumeter() {
  for (int x = 0; x < NUM_STRIPS; x++) {
    for (int i = 0; i < num_leds_per_strip; i++) {
      leds[x][i] = tiffanyBlueColor;
    }
    for (int j = NUM_LEDS_PER_STRIP - 1; j >= num_leds_per_strip; j--) {
      leds[x][j] = CRGB(0, 0, 0);
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
  FastLED.setBrightness(0);
  FastLED.show();
}

void pulsate() {

  //  float pulseSpeed = map(myRawMaxRPM, MINRPM, MAXRPM, 2, 50); //set the pulse speed range here, integer values only
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
      leds[x][i] = pulseColor;
    }
  }
  FastLED.setBrightness(int(pulseBrightLevel));
  FastLED.show();
}
