#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();


// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
const int FlashChipSelect = 6; // digital pin for flash chip CS pin

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlaySerialflashRaw    sound0;
AudioPlaySerialflashRaw    sound1;  // six memory players, so we can play
AudioPlaySerialflashRaw    sound2;  // all six sounds simultaneously
AudioPlaySerialflashRaw    sound3;
AudioPlaySerialflashRaw    sound4;
AudioPlaySerialflashRaw    sound5;
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

void setup() {
  Serial.println("Setup");
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
  
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);
  
  SPI.setSCK(14);  // Audio shield has SCK on pin 14
  SPI.setMISO(12);
  SPI.setMOSI(7);  // Audio shield has MOSI on pin 7

  if (!SerialFlash.begin(FlashChipSelect)) {
    error("Unable to access SPI Flash chip");
  }else{
    //error("Found SPI Flash chip");
  }
  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

 

 // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);
  
  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.5);
  mix1.gain(1, 0.5);
  mix1.gain(2, 0.5);
  mix1.gain(3, 0.5);
  mix2.gain(1, 0.5);
  mix2.gain(2, 0.5);

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
        Serial.print(i); Serial.println(" released");
          sound0.play("A1.WAV");
        break;
        case 1:
         sound1.play("B1.WAV");
        break;
        case 2:
          sound2.play("C1.WAV");
        break;
        case 3:
         sound3.play("D1.WAV");
        break;
        case 4:
          sound4.play("E1.WAV");
        break;
      
      }

   
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
  

}

void error(const char *message) {
  while (1) {
    Serial.println(message);
    delay(2500);
  }
}
