// initialise pins 

uint8_t touchPin[2] = {0, 1}; // make a list of the pins with aerials

// create stored values

int ntouch = 2; // used in for loop criteria, increase to use more touch pins
int touchValue = 0; // stored value for touch readings

void setup() {
  Serial.begin(38400); // enable serial
  delay(200);
}

void loop() {

  for(uint8_t i=0; i < ntouch; i++) {
    // for loop will scan each of the touch pins defined at the top before 
      // moving on, a new stored value "i" is created here and used below
        // to recall values from our list(s) at the top

  touchValue = touchRead(touchPin[i]); // touchValue is set by reading a pin
    // touchRead is part of the Teensy library

  Serial.print("touchPin");
  Serial.print(touchPin[i]);
  Serial.print(" ");
  Serial.print("Value");
  Serial.println(touchValue);

  // this will print out the value of each pin to the Serial Monitor
    // the Serial Monitor can be found in the Tools menu
      // set the baud rate to 38400

  delay(100); // for the sake of readability, this pauses each loop for 100ms

  }
}
