int activationCounter, //from 0 to 6. Tracks the number of relays activated. At state 6, the flood lights turn on.
    currActivationState, prevActivationState;

bool hasInitialised,
     isCoolingDown,
     isTimeToSample,
     isPlaying;

long isTurnedOffTime, lastSampledTime, isFullyActivatedTime;

//user settings
const int COOLDOWNDURATION = 15000, //duration after flood lights turn off to ignore rpm readings to let the wheels slow down
          SAMPLEPERIOD = 4000, //time between sampling the rpm tp update the relay states
          PLAYPERIOD = 8000; //duration of fully activated state

bool isDebug = false;

void update_relays() {

  update_logic_states(); //keeps track of logic states

  if ( hasInitialised && !isCoolingDown && isTimeToSample && !isPlaying) {

    update_activation_state(); //by sampling the rpm

    isTimeToSample = false;
  }

  write_to_relays();
}

void reset() {

  activationCounter = 0;
  isPlaying = false;
  hasInitialised = false;
  isCoolingDown = true;
  isTurnedOffTime = millis();

  turn_off_leds();
  turn_off_relays();

  if (isDebug) {
    Serial.println();
    Serial.print("reset and start cool down");
    Serial.println();
  }
}

void update_logic_states() {

  if (!hasInitialised && !isCoolingDown && !isPlaying) {
    if (myRawMaxRPM > MINRPM) {
      hasInitialised = true;

      if (isDebug) {
        Serial.println();
        Serial.print("has Initialised");
        Serial.println();
      }
    }
  }

  if (millis() - lastSampledTime > SAMPLEPERIOD) {
    isTimeToSample = true;
  }

  if (millis() - isFullyActivatedTime > PLAYPERIOD && isPlaying) {
    if (myRawMaxRPM > MAXRPM / 2) {
      isFullyActivatedTime = millis(); //play again
    } else {
      reset();
    }
  }

  if (millis() - isTurnedOffTime > COOLDOWNDURATION && isCoolingDown) {
    isCoolingDown = false;
    if (isDebug) {
        Serial.println();
        Serial.print("has cooled down");
        Serial.println();
    }
  }
}

void update_activation_state() { //sample the rpms

  if (myRawMaxRPM > MINRPM) {
    activationCounter++;
  } else {
    activationCounter--;
    if (activationCounter == 0) {
      hasInitialised = false;
      if (isDebug) {
        Serial.println();
        Serial.print("hasInitialised = false");
        Serial.println();
      }
    }
  }

  activationCounter = constrain(activationCounter, 0, 6);
  currActivationState = activationCounter;

  lastSampledTime = millis();

  if (isDebug) {
    Serial.println();
    Serial.print("activation counter: ");
    Serial.print(activationCounter);
    Serial.println();
  }
}

void write_to_relays() { //write out to the relays

  if (currActivationState != prevActivationState) { //only write if there is a change

    if (activationCounter == 0) {
      digitalWrite(floodRelayPin1, HIGH);
      digitalWrite(floodRelayPin2, HIGH);
      digitalWrite(motorRelayPin1, HIGH);
      digitalWrite(motorRelayPin2, HIGH);
      digitalWrite(motorRelayPin3, HIGH);
      digitalWrite(motorRelayPin4, HIGH);
      digitalWrite(motorRelayPin5, HIGH);
    } else if (activationCounter == 1) {
      digitalWrite(floodRelayPin1, HIGH);
      digitalWrite(floodRelayPin2, HIGH);
      digitalWrite(motorRelayPin1, LOW);
      digitalWrite(motorRelayPin2, HIGH);
      digitalWrite(motorRelayPin3, HIGH);
      digitalWrite(motorRelayPin4, HIGH);
      digitalWrite(motorRelayPin5, HIGH);
    } else if (activationCounter == 2) {
      digitalWrite(floodRelayPin1, HIGH);
      digitalWrite(floodRelayPin2, HIGH);
      digitalWrite(motorRelayPin1, LOW);
      digitalWrite(motorRelayPin2, LOW);
      digitalWrite(motorRelayPin3, HIGH);
      digitalWrite(motorRelayPin4, HIGH);
      digitalWrite(motorRelayPin5, HIGH);
    } else if (activationCounter == 3) {
      digitalWrite(floodRelayPin1, HIGH);
      digitalWrite(floodRelayPin2, HIGH);
      digitalWrite(motorRelayPin1, LOW);
      digitalWrite(motorRelayPin2, LOW);
      digitalWrite(motorRelayPin3, LOW);
      digitalWrite(motorRelayPin4, HIGH);
      digitalWrite(motorRelayPin5, HIGH);
    } else if (activationCounter == 4) {
      digitalWrite(floodRelayPin1, HIGH);
      digitalWrite(floodRelayPin2, HIGH);
      digitalWrite(motorRelayPin1, LOW);
      digitalWrite(motorRelayPin2, LOW);
      digitalWrite(motorRelayPin3, LOW);
      digitalWrite(motorRelayPin4, LOW);
      digitalWrite(motorRelayPin5, HIGH);
    } else if (activationCounter == 5) {
      digitalWrite(floodRelayPin1, HIGH);
      digitalWrite(floodRelayPin2, HIGH);
      digitalWrite(motorRelayPin1, LOW);
      digitalWrite(motorRelayPin2, LOW);
      digitalWrite(motorRelayPin3, LOW);
      digitalWrite(motorRelayPin4, LOW);
      digitalWrite(motorRelayPin5, LOW);
    } else if (activationCounter == 6) {
      digitalWrite(floodRelayPin1, LOW);
      digitalWrite(floodRelayPin2, LOW);
      digitalWrite(motorRelayPin1, LOW);
      digitalWrite(motorRelayPin2, LOW);
      digitalWrite(motorRelayPin3, LOW);
      digitalWrite(motorRelayPin4, LOW);
      digitalWrite(motorRelayPin5, LOW);

      isFullyActivatedTime = millis();
      isPlaying = true;
      turn_off_leds();

      if (isDebug) {
        Serial.println();
        Serial.print("is playing");
        Serial.println();
      }
    }

    prevActivationState = currActivationState;
  }
}

void turn_off_relays() {
  digitalWrite(floodRelayPin1, HIGH);
  digitalWrite(floodRelayPin2, HIGH);
  digitalWrite(motorRelayPin1, HIGH);
  digitalWrite(motorRelayPin2, HIGH);
  digitalWrite(motorRelayPin3, HIGH);
  digitalWrite(motorRelayPin4, HIGH);
  digitalWrite(motorRelayPin5, HIGH);
}













