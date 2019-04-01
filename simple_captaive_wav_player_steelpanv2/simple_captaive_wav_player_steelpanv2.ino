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

String sounds[]={"C1.TRW","D1.TRW","E1.TRW","F1.TRW","G1.TRW","A1.TRW","B1.TRW","C2.TRW"};

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
const int PLAYERS=16;

// GUItool: begin automatically generated code
AudioPlaySerialflashRaw  sound[]={
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw(),
  AudioPlaySerialflashRaw()
 };         //xy=123,44

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code

AudioMixer4              mix5; //xy=284,96
AudioMixer4              mix7; //xy=296,547
AudioMixer4              mix6; //xy=310,331
AudioMixer4              mix8; //xy=312,782
AudioMixer4              mix9; //xy=457,269
AudioOutputAnalog        dac;            //xy=612,246
AudioOutputI2S           headphones;     //xy=629,195
AudioConnection          patchCord1(sound[0], 0, mix5, 1);
AudioConnection          patchCord2(sound[1], 0, mix7, 1);
AudioConnection          patchCord3(sound[2], 0, mix5, 0);
AudioConnection          patchCord4(sound[3], 0, mix6, 1);
AudioConnection          patchCord5(sound[4], 0, mix7, 0);
AudioConnection          patchCord6(sound[5], 0, mix8, 1);
AudioConnection          patchCord7(sound[6], 0, mix5, 2);
AudioConnection          patchCord8(sound[7], 0, mix6, 0);
AudioConnection          patchCord9(sound[8], 0, mix7, 3);
AudioConnection          patchCord10(sound[9], 0, mix8, 0);
AudioConnection          patchCord11(sound[10], 0, mix7, 2);
AudioConnection          patchCord12(sound[11], 0, mix6, 3);
AudioConnection          patchCord13(sound[12], 0, mix5, 3);
AudioConnection          patchCord14(sound[13], 0, mix6, 2);
AudioConnection          patchCord15(sound[14], 0, mix8, 3);
AudioConnection          patchCord16(sound[15], 0, mix8, 2);
AudioConnection          patchCord17(mix5, 0, mix9, 0);
AudioConnection          patchCord18(mix7, 0, mix9, 2);
AudioConnection          patchCord19(mix6, 0, mix9, 1);
AudioConnection          patchCord20(mix8, 0, mix9, 3);
AudioConnection          patchCord21(mix9, 0, headphones, 1);
AudioConnection          patchCord22(mix9, 0, headphones, 0);
AudioConnection          patchCord23(mix9, dac);
AudioControlSGTL5000     audioShield;    //xy=623,289
// GUItool: end automatically generated code




char* string2char(String str){
    if(str.length()!=0){
        char *p = const_cast<char*>(str.c_str());
        return p;
    }
}

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
  audioShield.volume(0.8);

  sound[0].play("C1.TRW");
  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  for (uint8_t i=0; i<4; i++) {
    mix5.gain(i, 0.25);
    mix6.gain(i, 0.25);
    mix7.gain(i, 0.25);
    mix8.gain(i, 0.25);
    mix9.gain(i, 0.25);
  } 
}

void loop() {

  // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<16; i++) {
  // cap.setThreshholds(40,1);
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    int count_active=0;
    bool found =false;
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
      for (uint8_t j=0; j<PLAYERS; j++) {
        if(!sound[j].isPlaying()){
          Serial.print(j); Serial.println(" playing");
          sound[j].play(string2char(sounds[i]));
          found=true;
          break;
        }
      }
      //if(found==false){
      //  sound[0].play(string2char(sounds[i]));
     // }
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
