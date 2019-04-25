/* Buttons to USB Keyboard Example

   You must select Keyboard from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#include <Bounce.h>

// Create Bounce objects for each button.  The Bounce object
// automatically deals with contact chatter or "bounce", and
// it makes detecting changes very simple.
Bounce button0 = Bounce(1, 10);
Bounce button1 = Bounce(2, 10);  // 10 = 10 ms debounce time
Bounce button2 = Bounce(3, 10);  // which is appropriate for
Bounce button3 = Bounce(4, 10);  // most mechanical pushbuttons
Bounce button4 = Bounce(5, 10);
Bounce button5 = Bounce(6, 10);  // if a button is too "sensitive"


void setup() {
  // Configure the pins for input mode with pullup resistors.
  // The pushbuttons connect from each pin to ground.  When
  // the button is pressed, the pin reads LOW because the button
  // shorts it to ground.  When released, the pin reads HIGH
  // because the pullup resistor connects to +5 volts inside
  // the chip.  LOW for "on", and HIGH for "off" may seem
  // backwards, but using the on-chip pullup resistors is very
  // convenient.  The scheme is called "active low", and it's
  // very commonly used in electronics... so much that the chip
  // has built-in pullup resistors!
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);  // Teensy++ LED, may need 1k resistor pullup
  pinMode(6, INPUT_PULLUP);
}

void loop() {
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.

  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();
  
  if (button0.fallingEdge()) {
     Keyboard.press(KEY_D);
    //Keyboard.release(KEY_D);
  }
  if (button1.fallingEdge()) {
    Keyboard.press(KEY_A);
   // Keyboard.release(KEY_A);
  }
  if (button2.fallingEdge()) {
    Keyboard.press(KEY_F5);
   // Keyboard.release(KEY_F5);
  }
  if (button3.fallingEdge()) {
    Keyboard.press(KEY_J);
    //Keyboard.release(KEY_J);
  }
  if (button4.fallingEdge()) {
    Keyboard.press(KEY_QUOTE);
   // Keyboard.release(KEY_COMMA);
  }
  if (button5.fallingEdge()) {
    Keyboard.press(KEY_BACKSPACE);
   // Keyboard.release(KEY_BACKSPACE);
  }


  if (button0.risingEdge()) {
   //  Keyboard.press(KEY_D);
    Keyboard.release(KEY_D);
  }
  if (button1.risingEdge()) {
    //Keyboard.press(KEY_A);
    Keyboard.release(KEY_A);
  }
  if (button2.risingEdge()) {
    //Keyboard.press(KEY_F5);
    Keyboard.release(KEY_F5);
  }
  if (button3.risingEdge()) {
   // Keyboard.press(KEY_J);
    Keyboard.release(KEY_J);
  }
  if (button4.risingEdge()) {
   // Keyboard.press(KEY_COMMA);
    Keyboard.release(KEY_QUOTE);
  }
  if (button5.risingEdge()) {
   // Keyboard.press(KEY_BACKSPACE);
    Keyboard.release(KEY_BACKSPACE);
  }
}
