#include <Wire.h>
#include "Adafruit_MPR121.h"

#include <FastLED.h>

#define NUM_LEDS 1

CRGB leds[NUM_LEDS];
#define DATA_PIN 3
#define CLOCK_PIN 13
elapsedMillis timer = 0;
int delayValue = 5;
#include <Audio.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
long nextBeat = 0;
byte byteIndex = 0;
//
//// GUItool: begin automatically generated code
AudioMixer4              mixer1;
AudioMixer4              mixer2;
AudioMixer4              mixer3;
AudioMixer4              mixer4;
AudioMixer4              masterMixer;

AudioPlaySerialflashRaw  sound[16];
AudioOutputI2S           lineout;           //xy=789.9999999999998,185.99999999999997

AudioConnection          c1(sound[0], 0, mixer1, 0);
AudioConnection          c2(sound[1], 0, mixer1, 1);
AudioConnection          c3(sound[2], 0, mixer1, 2);
AudioConnection          c4(sound[3], 0, mixer1, 3);
AudioConnection          c5(sound[4], 0, mixer2, 0);
AudioConnection          c6(sound[5], 0, mixer2, 1);
AudioConnection          c7(sound[6], 0, mixer2, 2);
AudioConnection          c8(sound[7], 0, mixer2, 3);
AudioConnection          c9(sound[8], 0, mixer3, 0);
AudioConnection          c10(sound[9], 0, mixer3, 1);
AudioConnection          c11(sound[10], 0, mixer3, 2);
AudioConnection          c12(sound[11], 0, mixer3, 3);
AudioConnection          c13(sound[12], 0, mixer4, 0);
AudioConnection          c14(sound[13], 0, mixer4, 1);
AudioConnection          c15(sound[14], 0, mixer4, 2);
AudioConnection          c16(sound[15], 0, mixer4, 3);

AudioConnection          mix1(mixer1, 0, masterMixer, 0);
AudioConnection          mix2(mixer2, 0, masterMixer, 1);
AudioConnection          mix3(mixer3, 0, masterMixer, 2);
AudioConnection          mix4(mixer4, 0, masterMixer, 3);

AudioConnection          masterL(masterMixer, 0, lineout, 0);
AudioConnection          masterR(masterMixer, 0, lineout, 1);

AudioControlSGTL5000     codec;
//// GUItool: end automatically generated code

#define NUM_ELECTRODES 12

//touch and release thresholds
#define TOUCH 40
#define RELEASE 1

byte cycle;
bool triggerToggle[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
byte pattern[16]={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
String soundFile[16] = {"A1.WAV", "B1.WAV", "C1.WAV", "D1.WAV", "E1.WAV", "F1.WAV", "G1.WAV", "C1.WAV"};
Adafruit_MPR121 cap5A = Adafruit_MPR121();
Adafruit_MPR121 cap5C = Adafruit_MPR121();

uint16_t lasttouched5A = 0;  //b0000000000000000
uint16_t currtouched5A = 0;  //b0000000000000000
uint16_t lasttouched5C = 0;  //b0000000000000000
uint16_t currtouched5C = 0;  //b0000000000000000

const int FlashChipSelect = 6; // digital pin for flash chip CS pin


void setup() {

  Serial.begin(115200);

  codec.enable();
  codec.volume(1);

  AudioMemory(120);

  SPI.setSCK(14);  // Audio shield has SCK on pin 14
  SPI.setMISO(12);
  SPI.setMOSI(7);  // Audio shield has MOSI on pin 7

  cap5A.begin(0x5A);
  cap5C.begin(0x5C);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  delay(20);

  for (byte i = 0; i < 4; i++) {
    float gainLevel = 0.4;
    mixer1.gain(i, gainLevel);
    mixer2.gain(i, gainLevel);
    mixer3.gain(i, gainLevel);
    mixer4.gain(i, gainLevel);
    masterMixer.gain(i, gainLevel);
  }

  //    if (!cap5A.begin(0x5A)) {
  //      Serial.println("MPR121 not found, check wiring A?");
  //      while (1);
  //    }
  //
  //    if (!cap5C.begin(0x5C)) {
  //      Serial.println("MPR121 not found, check wiring C?");
  //      while (1);
  //
  //      Serial.println("MPR121 found!");
  //    }

  //cap5A.setThresholds(TOUCH, RELEASE);
  //cap5C.setThresholds(TOUCH, RELEASE);

  // wait for Arduino Serial Monitor
  while (!Serial) ;
  delay(100);
  Serial.println("All Files on SPI Flash chip:");

  if (!SerialFlash.begin(FlashChipSelect)) {
    error("Unable to access SPI Flash chip");
  }

  SerialFlash.opendir();
  while (1) {
    char filename[64];
    uint32_t filesize;
    if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
      Serial.print("  ");
      Serial.print(filename);
      spaces(20 - strlen(filename));
      Serial.print("  ");
      Serial.print(filesize);
      Serial.print(" bytes");
      Serial.println();
    } else {
      break; // no more files
    }
  }

  // file test: just to verify we have the right files
  SerialFlashFile file;
  file = SerialFlash.open("A1.WAV");
  delay(5000);
  if (!file) {  // true if the file exists
    Serial.println ("File not found :(");
  }
  if (file) {  // true if the file exists
    Serial.println ("File found! :D");
    sound[0].play("A1.WAV");
     sound[1].play("C1.WAV");
  }
}


void loop() {
  if (timer >= nextBeat) {
    for (byte i = 0; i < 16; i++) {
      if (triggerToggle) {
        if (bitRead(pattern[byteIndex], pattern[i]) == 1) {
          char charBuf[100];
        //  soundFile[0].toCharArray(charBuf, 50);
         // sound[0].play(charBuf);
          //leds[i] = CRGB::White;
          //FastLED.show();
        } else {
          //leds[i] = CRGB::Red;
        }
      }
    }
    nextBeat = timer + delayValue;
    byteIndex++;
    if (byteIndex == 16) byteIndex = 0;
  }
}

void spaces(int num) {
  for (int i = 0; i < num; i++) {
    Serial.print(" ");
  }
}

void error(const char *message) {
  while (1) {
    Serial.println(message);
    delay(2500);
  }
}
