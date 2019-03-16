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


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playWav1;       //xy=113,427
AudioPlayMemory          sound0;         //xy=157,108
AudioPlayMemory          sound2;         //xy=172,202
AudioPlayMemory          sound5;         //xy=177,350
AudioPlayMemory          sound1;         //xy=179,160
AudioPlayMemory          sound3;         //xy=201,244
AudioPlayMemory          sound4;         //xy=210,287
AudioMixer4              mix1;           //xy=585,219
AudioMixer4              mix2;           //xy=746,370
AudioOutputAnalog        dac;            //xy=1089,368
AudioOutputI2S           headphones;     //xy=1121,204
AudioConnection          patchCord1(playWav1, 0, mix2, 3);
AudioConnection          patchCord2(sound0, 0, mix1, 0);
AudioConnection          patchCord3(sound2, 0, mix1, 2);
AudioConnection          patchCord4(sound5, 0, mix2, 2);
AudioConnection          patchCord5(sound1, 0, mix1, 1);
AudioConnection          patchCord6(sound3, 0, mix1, 3);
AudioConnection          patchCord7(sound4, 0, mix2, 1);
AudioConnection          patchCord8(mix1, 0, mix2, 0);
AudioConnection          patchCord9(mix2, 0, headphones, 0);
AudioConnection          patchCord10(mix2, 0, headphones, 1);
AudioConnection          patchCord11(mix2, dac);
AudioControlSGTL5000     audioShield;    //xy=887,538
// GUItool: end automatically generated code


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
const int TRIGGER_COUNT=6;

//Used to read the input pins
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
const int POT_PIN=A7;

typedef struct {
  Bounce butt;
  int soundid=0;
  int index=0;
  int toggle=0;
  elapsedMillis elapse;
}InputTrigger;

//Set up a new button
int pattern[][PATTERN_LENGTH]={
  {0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,0,0},
  {0,0,0,0,1,1,1,1},
  {1,0,0,1,0,0,1,0},
  {1,0,0,0,0,0,0,0},
  };

  // Use these with the Teensy Audio Shield
  #define SDCARD_CS_PIN    10
  #define SDCARD_MOSI_PIN  7
  #define SDCARD_SCK_PIN   14

InputTrigger my_trigger1={ Bounce(0, 5),SNARE};
InputTrigger my_trigger2={ Bounce(1, 5),TOM};
InputTrigger my_trigger3={ Bounce(2, 5),HIHAT};
InputTrigger my_trigger4={ Bounce(3, 5),KICK};
InputTrigger my_trigger5={ Bounce(4, 5),GONG};
InputTrigger my_trigger6={ Bounce(5, 5),REGISTER};



InputTrigger triggers[] = {
  my_trigger1,
  my_trigger2,
  my_trigger3,
  my_trigger4,
  my_trigger5,
  my_trigger6,
};

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

void setup() {
  Serial.begin(9600);
  delay(500);
  
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  Serial.println("Reading card");
  
   
  //clear the analogue
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
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
  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);
  mix1.gain(2, 0.4);
  mix1.gain(3, 0.4);
  mix2.gain(1, 0.4);
  mix2.gain(2, 0.4);
  mix2.gain(3, 0.3);
}



void loop() {
  int res=ReadPot();
  //map(value, fromLow, fromHigh, toLow, toHigh)
  int bpm=map(res, 1, 1023, 20, 250);
  
  if(!playWav1.isPlaying()){
    Serial.println("playing jingle");
    playWav1.play("jingle.wav");
  }
  

  
  for(int i=0;i<TRIGGER_COUNT;i++){
    triggers[i].butt.update();
  
    if (triggers[i].butt.fallingEdge()) {
      triggers[i].toggle=!triggers[i].toggle;
      triggers[i].elapse=0;
    }
    if(triggers[i].toggle==1){
      if (triggers[i].elapse > bpm){
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
  int countFile(File dir) {
    int count=0;
    while (true) {
  
      File entry =  dir.openNextFile();
      if (! entry) {
        // no more files
        break;
      }
      if (!entry.isDirectory()) {
        count=count+1;
      } 
      entry.close();
    }
    return 
  }
}
