#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

// WAV files converted to code by wav2sketch
#include "AudioSampleA1.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleB1.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleC1.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleD1.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "AudioSampleE1.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleF1.h" // http://www.freesound.org/people/kiddpark/sounds/201159/

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;


// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlayMemory    sound0;
AudioPlayMemory    sound1;  // six memory players, so we can play
AudioPlayMemory    sound2;  // all six sounds simultaneously
AudioPlayMemory    sound3;
AudioPlayMemory    sound4;
AudioPlayMemory    sound5;
AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioMixer4        mix2;    // tandem to combine 6 audio sources
AudioOutputI2S     headphones;
AudioOutputAnalog  dac;     // play to both I2S audio board and on-chip DAC

// Create Audio connections between the components
//
AudioConnection c1(sound0, 0, mix1, 0);
AudioConnection c2(sound1, 0, mix1, 1);
AudioConnection c3(sound2, 0, mix1, 2);
AudioConnection c4(sound3, 0, mix1, 3);
AudioConnection c5(mix1, 0, mix2, 0);   // output of mix1 into 1st input on mix2
AudioConnection c6(sound4, 0, mix2, 1);
AudioConnection c7(sound5, 0, mix2, 2);
AudioConnection c8(mix2, 0, headphones, 0);
AudioConnection c9(mix2, 0, headphones, 1);
AudioConnection c10(mix2, 0, dac, 0);

// Create an object to control the audio shield.
// 
AudioControlSGTL5000 audioShield;

// Bounce objects to read six pushbuttons (pins 0-5)
//
/*
Bounce button0 = Bounce(0, 5);
Bounce button1 = Bounce(1, 5);  // 5 ms debounce time
Bounce button2 = Bounce(2, 5);
Bounce button3 = Bounce(3, 5);
Bounce button4 = Bounce(4, 5);
Bounce button5 = Bounce(5, 5);
*/

void setup() {


  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
  
  // Configure the pushbutton pins for pullups.
  // Each button should connect from the pin to GND.
  //pinMode(0, INPUT_PULLUP);
 // pinMode(1, INPUT_PULLUP);
  //pinMode(2, INPUT_PULLUP);
 // pinMode(3, INPUT_PULLUP);
  //pinMode(4, INPUT_PULLUP);
  //pinMode(5, INPUT_PULLUP);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.8);
  mix1.gain(1, 0.8);
  mix1.gain(2, 0.8);
  mix1.gain(3, 0.8);
  mix2.gain(1, 0.8);
  mix2.gain(2, 0.8);


  
}

void loop() {

  // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<12; i++) {
  // cap.setThreshholds(40,1);
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {

switch(i){
  case 0:
    sound0.play(AudioSampleA1);
  break;
  case 1:
   sound1.play(AudioSampleB1);
  break;
  case 2:
    sound2.play(AudioSampleC1);
  break;
  case 3:
   sound3.play(AudioSampleD1);
  break;
  case 4:
    sound4.play(AudioSampleA1);
  break;

}



      
      Serial.print(i); Serial.println(" released");
    }
  }

  // reset our state
  lasttouched = currtouched;

  // comment out this line for detailed da//ta from the sensor!
  return;
  
  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();
  
  

  // When the buttons are pressed, just start a sound playing.
  // The audio library will play each sound through the mixers
  // so any combination can play simultaneously.
  //
  /*
  if (button0.fallingEdge()) {
    sound0.play(AudioSampleSnare);
  }
  if (button1.fallingEdge()) {
    sound1.play(AudioSampleTomtom);
  }
  if (button2.fallingEdge()) {
    sound2.play(AudioSampleHihat);
  }
  if (button3.fallingEdge()) {
    sound3.play(AudioSampleKick);
  }
  if (button4.fallingEdge()) {
    // comment this line to work with Teensy 3.0.
    // the Gong sound is very long, too much for 3.0's memory
    sound4.play(AudioSampleGong);
  }
  if (button5.fallingEdge()) {
   sound4.play(AudioSampleGong);
  }*/

}
