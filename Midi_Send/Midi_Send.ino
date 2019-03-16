#include <Bounce.h>  // Bounce library makes button change detection easy
const int channel = 1;
int ledPin = 13;
int buttonPin = 1;

Bounce button1 = Bounce(buttonPin, 5);  // 5 = 5 ms debounce time

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  button1.update();
  // Note On messages when each button is pressed
  if (button1.fallingEdge()) {
    digitalWrite(ledPin, HIGH);
    usbMIDI.sendNoteOn(60, 99, channel);  // 60 = C4
  }
  // Note Off messages when each button is released
  if (button1.risingEdge()) {
    digitalWrite(ledPin, LOW);
    usbMIDI.sendNoteOff(60, 0, channel);  // 60 = C4
  }
 
  // MIDI Controllers should discard incoming MIDI messages.
  while (usbMIDI.read()) {
  }
}
