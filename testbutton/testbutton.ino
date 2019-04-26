#include <Bounce.h>
const int buttonFill = 2;
const int buttonPlay = 3;
const int buttonBankUp = 5;
const int buttonBankDown = 4;

int buttonPin=buttonBankDown;

Bounce pushbutton = Bounce(buttonPin, 5);  // 10 ms debounce
void setup() {
  Serial.begin(57600);
  // put your setup code here, to run once:
pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Pushbutton Bounce library test:");
}

void loop() {
  pushbutton.update();
  // put your main code here, to run repeatedly:
if (pushbutton.fallingEdge()) {
     Serial.println("Pressed");
    }
}
