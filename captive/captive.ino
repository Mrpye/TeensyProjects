#include <CapacitiveSensor.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

int led_pin =13;
uint8_t touchPin[2] = {0, 1}; // make a list of the pins with aerials
int touchValue = 0; // stored value for touch readings     
AudioPlaySdWav           playWav1;
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14



void setup()                    
{
   //cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   //cs_4_6.set_CS_AutocaL_Millis(0xFFFFFFFF); 
   pinMode(led_pin, OUTPUT);  
   AudioMemory(8);
   Serial.begin(9600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  
}

void loop()                    
{
    //long start = millis();
    //long total1 =  cs_4_2.capacitiveSensor(30);
    //long total2 =  cs_4_6.capacitiveSensor(30);
   // long total3 =  cs_4_8.capacitiveSensor(30);
   long total1 = touchRead(23); 
   long total2 = touchRead(22); 
   Serial.print("total1");
  Serial.print(total1);
  Serial.print(" ");
  Serial.print("total2");
  Serial.println(total2);

    if(total1>3000 ){
      playFile("SDTEST1.WAV");  // filenames are always uppercase 8.3 format
    }else if(total2>3000){
      if(playWav1.isPlaying()){
        playWav1.stop();
      }
    }
    

    delay(10);                             // arbitrary delay to limit data to serial port 
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);

  // Simply wait for the file to finish playing.
  //while (playWav1.isPlaying()) {
    
  //}
}
