const int SINGLEMAXRPM = 1; //triggered by the max rpm of three bicycles
const int COMBINEDRPM = 2; //triggered by the combined rpm

//user settings
int whichMotorMode = SINGLEMAXRPM;

void calc_RPM() {
    
  for (int i = 0; i < 3; i++) {
    
    read_hall_sensor(i);

//    print_hall(i); //for debugging

    get_RPM(i);
  }

//  print_counter();//for debugging

//  print_RPM();//for debugging

  if (whichMotorMode == SINGLEMAXRPM) {
    get_mySingleMaxRPM();
  } else if (whichMotorMode == COMBINEDRPM) {
    get_myAverageRPM();
  }
}

void print_hall(int _i) {
  Serial.print("hall[");
  Serial.print(_i);
  Serial.print("]: ");
  Serial.print(currVal[_i]);
  Serial.print(" ");
}

void print_counter() {
  Serial.print("counter: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(counter[i]);
    Serial.print(" ");
  }
  Serial.print(" ");
}

void print_RPM() {
  Serial.print("RPM: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(RPM[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void read_hall_sensor(int i) {

  currVal[i] = digitalRead(i * 4 + 22);

  if (currVal[i] == LOW && prevVal[i] == HIGH) { //positive hall detection

    counter[i]++;
    isTriggeredTime[i] = millis();
  }

  prevVal[i] = currVal[i];
}

void get_RPM(int i) {

  timeInterval[i] = millis() - prevReadTime[i]; //independent of whether triggered or not

  oneRevTimeInterval[i] = isTriggeredTime[i] - prevReadTime[i];

  prevReadTime[i] = isTriggeredTime[i];

  if (oneRevTimeInterval[i] > 0) {

    RPM[i] = int( 60000.0 / oneRevTimeInterval[i]);

  }

  if (timeInterval[i] > 5000) { //ideally the bike wheel should slow down instead of abruptly stopping, but just in case
    RPM[i] = 0;
  }
}

void get_mySingleMaxRPM() {

  if ( RPM[0] > RPM[1] ) {
    myMaxRPM = RPM[0];
  } else {
    myMaxRPM = RPM[1];
  }

  if (myMaxRPM < RPM[2]) {
    myMaxRPM = RPM[2];
  }

  myMaxRPM = constrain(myMaxRPM, MINRPM, MAXRPM);
  myRawMaxRPM = myMaxRPM;
  //600 is analog value for the motor controller for cruise speed, 300 for max speed
  //update: set to 400 for slower max speed to reduce mechanical vibration
  myMaxRPM = map(myMaxRPM, MINRPM, MAXRPM, 600, 400);
}

void get_myAverageRPM() {

  myMaxRPM = (RPM[0] + RPM[1] + RPM[2]) / 3;
  myMaxRPM = constrain(myMaxRPM, MINRPM, MAXRPM);
  myRawMaxRPM = myMaxRPM;
  myMaxRPM = map(myMaxRPM, MINRPM, MAXRPM, 600, 400);
}

void update_motor_speed() {
  //to the 28 ebike motor controllers

  if (myMaxRPM != myPrevMaxRPM) {
    analogWrite(motorPin1, myMaxRPM);
    analogWrite(motorPin2, myMaxRPM);
    analogWrite(motorPin3, myMaxRPM);
    analogWrite(motorPin4, myMaxRPM);
    analogWrite(motorPin5, myMaxRPM);
    analogWrite(motorPin6, myMaxRPM);
    analogWrite(motorPin7, myMaxRPM);
    analogWrite(motorPin8, myMaxRPM);
    analogWrite(motorPin9, myMaxRPM);
    analogWrite(motorPin10, myMaxRPM);
    analogWrite(motorPin11, myMaxRPM);
    analogWrite(motorPin12, myMaxRPM);
    analogWrite(motorPin13, myMaxRPM);
    analogWrite(motorPin14, myMaxRPM);

    analogWrite(motorOutPin, myMaxRPM);
  }
  myPrevMaxRPM = myMaxRPM;
}

void use_pot() { //for simulation only

  //    Serial.print("pot reading: ");
  //    Serial.println(analogRead(potPin));

  myRawMaxRPM = map(analogRead(potPin), 0, 1023, MINRPM, MAXRPM);

  //  Serial.print("myRawMaxRPM: ");
  //  Serial.println(myRawMaxRPM);
}


