#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// *****************************************
// WAV files converted to code by wav2sketch
// *****************************************
#include "AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/


// **************
// Player Objects
// **************
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

// ***********************************************
// Create Audio connections between the components
// ***********************************************
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

// *********************************************
// Create an object to control the audio shield.
// *********************************************
AudioControlSGTL5000 audioShield;

// *****************************************
// Bounce objects to read six pushbuttons (pins 0-5)
// *****************************************
Bounce button0 = Bounce(0, 5);
Bounce button1 = Bounce(1, 5);  // 5 ms debounce time
Bounce button2 = Bounce(2, 5);
Bounce button3 = Bounce(3, 5);
Bounce button4 = Bounce(4, 5);
Bounce button5 = Bounce(5, 5);


const int SNARE=0;
const int TOM=1;
const int HIHAT=2;
const int KICK=3;
const int GONG=4;
const int REGISTER=5;

const int PATTERN_LENGTH=8;
const int TRIGGER_COUNT=2;

typedef struct {
  Bounce butt;
  int soundid=0;
  int touch=0;
  int touch_val=0;
  int index=0;
  int toggle=0;
  
  elapsedMillis elapse;
}InputTrigger;

//Set up a new button
int pattern[][PATTERN_LENGTH]={
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1}
  };
                                
InputTrigger my_trigger1={ Bounce(0, 5),SNARE,16};
InputTrigger my_trigger2={ Bounce(1, 5),TOM,17};



InputTrigger triggers[] = {my_trigger1,my_trigger2};


void setup() {
 
  Serial.begin(9600);

  // Configure the pushbutton pins for pullups.
  // Each button should connect from the pin to GND.
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

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
  mix1.gain(0, 0.6);
  mix1.gain(1, 0.6);
  mix1.gain(2, 0.6);
  mix1.gain(3, 0.6);
  mix2.gain(1, 0.6);
  mix2.gain(2, 0.6);
}
long touch_count=0;

void loop() {
  long touch_read=0;
  //Touch reading
  touch_count=touch_count+1;
  if(touch_count>10000){
    touch_count=0;
    for(int i=0;i<TRIGGER_COUNT;i++){
      triggers[i].touch_val=touchRead(triggers[i].touch);
      Serial.println(  triggers[i].touch_val );
        //triggers[i].touch);
    }
  }else{
    for(int i=0;i<TRIGGER_COUNT;i++){
      triggers[i].touch_val=0;
    }
  }

  for(int i=0;i<TRIGGER_COUNT;i++){
    triggers[i].butt.update();
    
    
    
    if (triggers[i].butt.fallingEdge()||triggers[i].touch_val>1500) {
      triggers[i].touch_val=0;
      triggers[i].toggle=!triggers[i].toggle;
      triggers[i].elapse=0;
    }
    if(triggers[i].toggle==1){
      if (triggers[i].elapse > 250){
        if(pattern[i][triggers[i].index]==1){

          switch (triggers[i].soundid) {
            case SNARE:
              sound0.play(AudioSampleSnare);
              break;
            case TOM:
              sound1.play(AudioSampleTomtom);
              break;
             case HIHAT:
              sound2.play(AudioSampleHihat);
              break;
            case KICK:
              sound3.play(AudioSampleKick);
              break;
            case GONG:
              sound4.play(AudioSampleGong);
              break;
            case REGISTER:
              sound5.play(AudioSampleCashregister);
              break;
          }
        }
        triggers[i].index=triggers[i].index+1;
        if(triggers[i].index>PATTERN_LENGTH-1){triggers[i].index=0;}
        triggers[i].elapse=0;
      } 
    } 
  }

}
