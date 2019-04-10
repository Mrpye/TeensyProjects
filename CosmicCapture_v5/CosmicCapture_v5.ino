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
#include "AudioSampleAgogo1.h"
#include "AudioSampleAgogo2.h"
#include "AudioSampleAgogo3.h"
#include "AudioSampleBoo.h"
#include "AudioSampleChime1.h"
#include "AudioSampleChime2.h"
#include "AudioSampleChime3.h"
#include "AudioSampleChime4.h"
#include "AudioSampleChime5.h"
#include "AudioSampleChime6.h"
#include "AudioSampleClave.h"
#include "AudioSampleClick.h"
#include "AudioSampleKiss.h"
#include "AudioSampleShaker1.h"
#include "AudioSampleShaker2.h"
#include "AudioSampleWawwaw.h"

const float vol = 1;
const int PATTERN_LENGTH = 32;
const int TRIGGER_COUNT = 16;
const int PLAYERS = 32;

//Used to read the input pins
const int numReadings = 5;
int readings[numReadings]; // the readings from the analog input
int readIndex = 0;         // the index of the current reading
int total = 0;             // the running total
int average = 0;           // the average
const int POT_PIN = A7;
int current_pattern = 0;

#ifndef _BV1
#define _BV1(bit) (1 << (bit))
#endif

#ifndef _BV2
#define _BV2(bit) (1 << (bit))
#endif
// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched1 = 0;
uint16_t currtouched1 = 0;
uint16_t lasttouched2 = 0;
uint16_t currtouched2 = 0;
elapsedMillis elapse;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();

AudioPlayMemory sound[] = {
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory(),
    AudioPlayMemory()}; //xy=123,44

// GUItool: begin automatically generated code
AudioMixer4 mix5;          //xy=284,96
AudioMixer4 mix7;          //xy=296,547
AudioMixer4 mix6;          //xy=310,331
AudioMixer4 mix8;          //xy=312,782
AudioMixer4 mix10;         //xy=333,1015
AudioMixer4 mix12;         //xy=345,1466
AudioMixer4 mix11;         //xy=359,1250
AudioMixer4 mix13;         //xy=361,1701
AudioMixer4 mix9;          //xy=457,271
AudioMixer4 mix14;         //xy=506,1188
AudioMixer4 mix15;         //xy=573,503

AudioOutputAnalog dac;     //xy=747,360
AudioOutputI2S headphones; //xy=764,309



AudioConnection patchCord1(sound[0], 0, mix5, 1);
AudioConnection patchCord2(sound[1], 0, mix7, 1);
AudioConnection patchCord3(sound[2], 0, mix5, 0);
AudioConnection patchCord4(sound[3], 0, mix6, 1);
AudioConnection patchCord5(sound[4], 0, mix7, 0);
AudioConnection patchCord6(sound[5], 0, mix8, 1);
AudioConnection patchCord7(sound[6], 0, mix5, 2);
AudioConnection patchCord8(sound[7], 0, mix6, 0);
AudioConnection patchCord9(sound[8], 0, mix7, 3);
AudioConnection patchCord10(sound[9], 0, mix8, 0);
AudioConnection patchCord11(sound[10], 0, mix7, 2);
AudioConnection patchCord12(sound[11], 0, mix10, 1);
AudioConnection patchCord13(sound[12], 0, mix6, 3);
AudioConnection patchCord14(sound[13], 0, mix5, 3);
AudioConnection patchCord15(sound[14], 0, mix6, 2);
AudioConnection patchCord16(sound[15], 0, mix8, 3);
AudioConnection patchCord17(sound[16], 0, mix8, 2);
AudioConnection patchCord18(sound[17], 0, mix12, 1);
AudioConnection patchCord19(sound[18], 0, mix10, 0);
AudioConnection patchCord20(sound[19], 0, mix11, 1);
AudioConnection patchCord21(sound[20], 0, mix12, 0);
AudioConnection patchCord22(sound[21], 0, mix13, 1);
AudioConnection patchCord23(sound[22], 0, mix10, 2);
AudioConnection patchCord24(sound[23], 0, mix11, 0);
AudioConnection patchCord25(sound[24], 0, mix12, 3);
AudioConnection patchCord26(sound[25], 0, mix13, 0);
AudioConnection patchCord27(sound[26], 0, mix12, 2);
AudioConnection patchCord28(sound[27], 0, mix11, 3);
AudioConnection patchCord29(sound[28], 0, mix10, 3);
AudioConnection patchCord30(sound[29], 0, mix11, 2);
AudioConnection patchCord31(sound[30], 0, mix13, 3);
AudioConnection patchCord32(sound[31], 0, mix13, 2);
AudioConnection patchCord33(mix5, 0, mix9, 0);
AudioConnection patchCord34(mix7, 0, mix9, 2);
AudioConnection patchCord35(mix6, 0, mix9, 1);
AudioConnection patchCord36(mix8, 0, mix9, 3);
AudioConnection patchCord37(mix10, 0, mix14, 0);
AudioConnection patchCord38(mix12, 0, mix14, 2);
AudioConnection patchCord39(mix11, 0, mix14, 1);
AudioConnection patchCord40(mix13, 0, mix14, 3);
AudioConnection patchCord41(mix9, 0, mix15, 0);
AudioConnection patchCord42(mix14, 0, mix15, 1);
AudioConnection patchCord43(mix15, 0, headphones, 0);
AudioConnection patchCord44(mix15, 0, headphones, 1);
AudioConnection patchCord45(mix15, dac);
AudioControlSGTL5000 audioShield; //xy=758,403
// GUItool: end automatically generated code

AudioMixer4 mixarray[]={mix5,mix7,mix5,mix6,mix7,mix8,mix5,mix6,mix7,mix8,mix7,mix10,mix6,mix5,mix6,mix8,mix8,mix12,
mix10,mix11,mix12,mix13,mix10,mix11,mix12,mix13,mix12,mix11,mix10,mix11,mix13,mix13};
int mixchannel[]={1,1,0,1,0,1,2,0,3,0,2,1,3,3,2,3,2,1,0,1,0,1,2,0,3,0,2,3,3,2,3,2};

const unsigned int *sounds[] = {
    AudioSampleClave,
    AudioSampleAgogo1,
    AudioSampleAgogo2,
    AudioSampleAgogo3,
    AudioSampleChime1,
    AudioSampleChime2,
    AudioSampleChime3,
    AudioSampleChime4,
    AudioSampleChime5,
    AudioSampleChime6,
    AudioSampleShaker1,
    AudioSampleShaker2,
    AudioSampleWawwaw,
    AudioSampleKiss,
    AudioSampleClick,
    AudioSampleBoo};

//Set up a new button
int pattern[][PATTERN_LENGTH] = {
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
};

bool toggle[TRIGGER_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void Rand_Pattern()
{
    for (uint8_t x = 0; x < TRIGGER_COUNT; x++)
    {
        for (uint8_t y = 0; y < PATTERN_LENGTH; y++)
        {
            pattern[x][y] = 1; // random(0, 2);
            Serial.println(pattern[x][y]);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Adafruit MPR121 Capacitive Touch sensor test");
    randomSeed(analogRead(POT_PIN));
    //Rand_Pattern();
    if (!cap1.begin(0x5B))
    {
        Serial.println("MPR121 1 not found, check wiring?");
        while (1)
            ;
    }
    Serial.println("MPR121 found 1 !");

    if (!cap2.begin(0x5A))
    {
        Serial.println("MPR121 2 not found, check wiring?");
        while (1)
            ;
    }
    Serial.println("MPR121 found 2!");

    AudioMemory(12);

    // turn on the output
    audioShield.enable();
    audioShield.volume(1);

    // reduce the gain on mixer channels, so more than 1
    // sound can play simultaneously without clipping
    Serial.println("Setting up Volume!");
    const float vol = 0.35;
    for (uint8_t i = 0; i < 4; i++)
    {
        mix5.gain(i, vol);
        mix6.gain(i, vol);
        mix7.gain(i, vol);
        mix8.gain(i, vol);
        mix9.gain(i, vol);
        mix10.gain(i, vol);
        mix11.gain(i, vol);
        mix12.gain(i, vol);
        mix13.gain(i, vol);
        mix14.gain(i, vol);
    }
    mix15.gain(0, 0.5);
    mix15.gain(1, 0.5);
    Serial.println("Volume Setup!");

    //cap1.setThresholds(5,5);
    //cap2.setThresholds(5,5);
}
long touch_count = 0;

void loop()
{

    //REad the touch values
    currtouched1 = cap1.touched();
    currtouched2 = cap2.touched();

    for (uint8_t i = 0; i < 12; i++)
    {
        if (!(currtouched1 & _BV1(i)) && (lasttouched1 & _BV1(i)))
        {
            Serial.print(i);
            Serial.println(" cap 1 released");
            toggle[i] = !toggle[i];
        }
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        if (!(currtouched2 & _BV2(i)) && (lasttouched2 & _BV2(i)))
        {
            Serial.print(i + 12);
            Serial.println(" cap 2 released");
            toggle[i + 12] = !toggle[i + 12];
        }
    }
    // reset our state
    lasttouched1 = currtouched1;
    lasttouched2 = currtouched2;

    int res = ReadPot();
    //map(value, fromLow, fromHigh, toLow, toHigh)
    int bpm = map(res, 1, 1023, 50, 200);

    if (elapse > bpm)
    {
        for (uint8_t i = 0; i < TRIGGER_COUNT; i++)
        {
            if (toggle[i] == 1 && pattern[i][current_pattern] == 1)
            {
                PlaySound(i);
            }
        }
        elapse = 0;
        current_pattern = current_pattern + 1;
        if (current_pattern > PATTERN_LENGTH)
        {
            current_pattern = 0;
        }
    }
}

void PlaySound(uint8_t i)
{
    for (uint8_t j = 0; j < PLAYERS; j++)
    {
        if (!sound[j].isPlaying())
        {
          
            Serial.print(i);
            Serial.println(" playing");
            mixarray[j].gain(mixchannel[j],1);
            );
            sound[j].play(sounds[i]);
            break;
        }
    }
}

int ReadPot()
{
    // subtract the last reading:
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = analogRead(POT_PIN);
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings)
    {
        // ...wrap around to the beginning:
        readIndex = 0;
    }

    // calculate the average:
    average = total / numReadings;
    // send it to the computer as ASCII digits
    return average;
}
