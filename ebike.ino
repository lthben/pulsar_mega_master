int currVal[3], prevVal[3], counter[3];
int RPM[3], //individual RPM for the three bicycles
    myMaxRPM, //the mapped analog value from the RawMaxRPM to be written to the motor controllers
    myPrevMaxRPM,
    myRawMaxRPM; //the actual Max RPM constrained between MINRPM and MAXRPM
long isTriggeredTime[3], prevReadTime[3];
long oneRevTimeInterval[3], timeInterval[3];

const int SINGLEMAXRPM = 1; //triggered by the max rpm of three bicycles
const int COMBINEDRPM = 2; //triggered by the combined rpm

//user settings
const int WHICHMOTORMODE = SINGLEMAXRPM; //safer choice in case of hall sensor breakdown
const int MINRPM = 20, MAXRPM = 100;

bool isDebugRPM = false;

void calc_RPM() {

  for (int i = 0; i < 3; i++) {

    read_hall_sensor(i);

    if (isDebugRPM) print_hall(i); //for debugging

    get_RPM(i);
  }

  if (isDebugRPM) print_counter();//for debugging

  if (isDebugRPM) print_RPM();//for debugging

  if (WHICHMOTORMODE == SINGLEMAXRPM) {
    get_mySingleMaxRPM();
  } else if (WHICHMOTORMODE == COMBINEDRPM) {
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

  if (timeInterval[i] > 3000) { //ideally the bike wheel should slow down instead of abruptly stopping, but just in case
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

void update_ebike_speed() {
  //to the 28 ebike motor controllers

  if (myMaxRPM != myPrevMaxRPM) {
    analogWrite(ebikePin1, myMaxRPM);
    analogWrite(ebikePin2, myMaxRPM);
    analogWrite(ebikePin3, myMaxRPM);
    analogWrite(ebikePin4, myMaxRPM);
    analogWrite(ebikePin5, myMaxRPM);
    analogWrite(ebikePin6, myMaxRPM);
    analogWrite(ebikePin7, myMaxRPM);
    analogWrite(ebikePin8, myMaxRPM);
    analogWrite(ebikePin9, myMaxRPM);
    analogWrite(ebikePin10, myMaxRPM);
    analogWrite(ebikePin11, myMaxRPM);
    analogWrite(ebikePin12, myMaxRPM);
    analogWrite(ebikePin13, myMaxRPM);
    analogWrite(ebikePin14, myMaxRPM);
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


