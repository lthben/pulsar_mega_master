const int SINGLEMAXRPM = 1; //triggered by the max rpm of three bicycles
const int COMBINEDRPM = 2; //triggered by the combined rpm 

//user settings
int whichMotorMode = SINGLEMAXRPM;

void use_pot() { //for simulation only
  //    Serial.print("pot reading: ");
  //    Serial.println(analogRead(potPin));

  myRawMaxRPM = map(analogRead(potPin), 0, 1023, MINRPM, MAXRPM);

//  Serial.print("myRawMaxRPM: ");
//  Serial.println(myRawMaxRPM);
}

void calc_RPM() {
    for (int i = 0; i < 3; i++) {
    read_hall_sensor(i);
    get_RPM(i);
  }

  if (whichMotorMode == SINGLEMAXRPM) {
    get_mySingleMaxRPM();
  } else {
    get_myAverageRPM();
  }
}

void read_hall_sensor(int i) {

  currVal[i] = digitalRead(i * 4 + 22);

//  Serial.print("hall: ");
//  for (int i=0; i<3; i++) {
//    Serial.print(currVal[i]);
//    Serial.print(" ");
//  }
//  Serial.println();

  if (currVal[i] == LOW && prevVal[i] == HIGH) { //positive hall detection

    counter[i]++;
    isTriggeredTime[i] = millis();

//        Serial.print("counter ");
//        Serial.print(i*4 + 22);
//        Serial.print(": ");
//        Serial.println(counter[i]);
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

  if (timeInterval[i] > 5000) {
    /*
       ideally the bike wheel should slow down instead of abruptly stopping,
       but just in case
    */
    RPM[i] = 0;
  }
}

void get_mySingleMaxRPM() {

//      Serial.print("RPM ");
//      for (int i=0; i<3; i++) {
//          Serial.print(RPM[i]);
//          Serial.print(" ");
//      }
//      Serial.println();

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

void update_motor_speed() { //to the 28 ebike motor controllers

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

