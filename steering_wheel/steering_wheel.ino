/* Simple USB Mouse Example
   Teensy becomes a USB mouse and moves the cursor in a triangle

   You must select Mouse from the "Tools > USB Type" menu

   This example code is in the public domain.
*/
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
const int POT_PIN=A7;


void setup() { } // no setup needed
void loop() {
  int i;
  for (i=0; i<40; i++) {
    Mouse.move(2, -1);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(2, 2);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(-4, -1);
    delay(25);
  }
}


int ReadPot(){
   // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(POT_PIN);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  return average;
}
