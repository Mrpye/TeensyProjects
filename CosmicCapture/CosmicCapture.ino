#include "Adafruit_MPR121.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>


// *****************************************
// WAV files converted to code by wav2sketch
// *****************************************
#include "AudioSampleSample1.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleSample2.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleSample3.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleSample4.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "AudioSampleSample5.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleSample6.h" // http://www.freesound.org/people/kiddpark/sounds/201159/



// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=159,524
AudioPlayMemory          sound0;         //xy=199,186
AudioPlayMemory          sound1;         //xy=201,245
AudioPlayMemory          sound4;         //xy=234,403
AudioPlayMemory          sound5;         //xy=235,459
AudioPlayMemory          sound2;         //xy=237,285
AudioPlayMemory          sound3;         //xy=248,346
AudioMixer4              mix1;           //xy=444,248
AudioEffectFreeverb      freeverb1;      //xy=654,229
AudioMixer4              mix2;           //xy=773,345
AudioOutputAnalog        dac;            //xy=1091,425
AudioOutputI2S           headphones;     //xy=1104,516
AudioConnection          patchCord1(playSdWav1, 1, mix2, 3);
AudioConnection          patchCord2(sound0, 0, mix1, 0);
AudioConnection          patchCord3(sound1, 0, mix1, 1);
AudioConnection          patchCord4(sound4, 0, mix2, 1);
AudioConnection          patchCord5(sound5, 0, mix2, 2);
AudioConnection          patchCord6(sound2, 0, mix1, 2);
AudioConnection          patchCord7(sound3, 0, mix1, 3);
AudioConnection          patchCord8(mix1, freeverb1);
AudioConnection          patchCord9(freeverb1, 0, mix2, 0);
AudioConnection          patchCord10(mix2, dac);
AudioConnection          patchCord11(mix2, 0, headphones, 0);
AudioConnection          patchCord12(mix2, 0, headphones, 1);
AudioControlSGTL5000     audioShield;    //xy=853,490
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


const int SOUND1=0;
const int SOUND2=1;
const int SOUND3=2;
const int SOUND4=3;
const int SOUND5=4;
const int SOUND6=5;

const int PATTERN_LENGTH=32;
const int TRIGGER_COUNT=6;

//Used to read the input pins
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
const int POT_PIN=A7;

#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
 
typedef struct {
  Bounce butt;
  int soundid=0;
  //int touch=0;
  int touch_state=0;
  int index=0;
  int toggle=0;
  
  elapsedMillis elapse;
}InputTrigger;

//Set up a new button
int pattern[][PATTERN_LENGTH]={
  {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0},
  {1,0,1,1,0,0,0,0,1,0,1,1,0,0,0,0,1,0,1,1,0,0,0,0,1,0,1,1,0,0,0,0},
  {1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,0},
  {1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0},
  {0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
  };
  
void Rand_Pattern(){

  for (uint8_t x=0; x<TRIGGER_COUNT; x++) {
    for (uint8_t y=0; y<PATTERN_LENGTH; y++) {
      pattern[x][y]=random(0, 2);
      Serial.println(pattern[x][y]);
    }                    
  }
}
InputTrigger my_trigger1={ Bounce(0, 5),SOUND1};
InputTrigger my_trigger2={ Bounce(1, 5),SOUND2};
InputTrigger my_trigger3={ Bounce(2, 5),SOUND3};
InputTrigger my_trigger4={ Bounce(3, 5),SOUND4};
InputTrigger my_trigger5={ Bounce(4, 5),SOUND5};
InputTrigger my_trigger6={ Bounce(5, 5),SOUND6};


InputTrigger triggers[] = {my_trigger1,my_trigger2,my_trigger3,my_trigger4,my_trigger5,my_trigger6};

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;


void setup() {
 
  //Serial.begin(9600);
  Serial.begin(115200);
 randomSeed(analogRead(POT_PIN));
Rand_Pattern();
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");


  
  SPI.setMOSI(SDCARD_MOSI_PIN);
 SPI.setSCK(SDCARD_SCK_PIN);
 if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
   }
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
  audioShield.volume(0.7);

  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.5);
  mix1.gain(1, 0.5);
  mix1.gain(2, 0.5);
  mix1.gain(3, 0.5);
  mix2.gain(1, 0.5);
  mix2.gain(2, 0.5);
  mix2.gain(3, 1);
  
  freeverb1.roomsize(1);
   freeverb1.damping(0.5);
}
long touch_count=0;

void loop() {

   if(! playSdWav1.isPlaying()){
    Serial.println("playing jingle");
     playSdWav1.play("jingle.wav");
  }

  currtouched = cap.touched();
  
  for (uint8_t i=0; i<TRIGGER_COUNT; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      triggers[i].touch_state=1;
      Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      triggers[i].touch_state=0;
      Serial.print(i); Serial.println(" released");
    }
  }

  // reset our state
  lasttouched = currtouched;


  int res=ReadPot();
//Serial.println(res);

  
  //map(value, fromLow, fromHigh, toLow, toHigh)
  int bpm=map(res, 1, 1023, 500, 5000);
  
  for(int i=0;i<TRIGGER_COUNT;i++){
    triggers[i].butt.update();

    if (triggers[i].butt.fallingEdge()||triggers[i].touch_state==1) {
      triggers[i].touch_state=0;
      triggers[i].toggle=!triggers[i].toggle;
      triggers[i].elapse=0;
    }
    if(triggers[i].toggle==1){
      if (triggers[i].elapse > bpm){
        if(pattern[i][triggers[i].index]==1){

          switch (triggers[i].soundid) {
            case SOUND1:
              sound0.play(AudioSampleSample1);
              break;
            case SOUND2:
              sound1.play(AudioSampleSample2);
              break;
             case SOUND3:
             Serial.print("bb");
              sound2.play(AudioSampleSample3);
              break;
            case SOUND4:
              sound3.play(AudioSampleSample4);
              break;
            case SOUND5:
              sound4.play(AudioSampleSample5);
              break;
            case SOUND6:
              sound5.play(AudioSampleSample6);
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
