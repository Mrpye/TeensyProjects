#include "Adafruit_MPR121.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>


#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
 
#ifndef _BVA
#define _BVA(bit) (1 << (bit)) 
#endif
#ifndef _BVB
#define _BVB(bit) (1 << (bit)) 
#endif
// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 capA = Adafruit_MPR121();
Adafruit_MPR121 capB = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouchedA = 0;
uint16_t currtouchedA = 0;
uint16_t lasttouchedB = 0;
uint16_t currtouchedB = 0;

void setup() {
 
  //Serial.begin(9600);
  Serial.begin(115200);
 
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor A test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!capA.begin(0x5A)) {
    Serial.println("MPR121 A not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 A found!");

  
  // Default address is 0x5B, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5B and if SCL then 0x5D
  if (!capB.begin(0x5B)) {
    Serial.println("MPR121 B not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 B found!");
}


void loop() {

  currtouchedA = capA.touched();
  currtouchedB = capB.touched();
  for (uint8_t i=0; i<13; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouchedA & _BVA(i)) && !(lasttouchedA & _BVA(i)) ) {
      
      Serial.print(i); Serial.println(" touched A");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouchedA & _BVA(i)) && (lasttouchedA & _BVA(i)) ) {

      Serial.print(i); Serial.println(" released A");
    }
  }
  for (uint8_t i=0; i<13; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouchedB & _BVB(i)) && !(lasttouchedB & _BVB(i)) ) {
      
      Serial.print(i); Serial.println(" touched B");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouchedB & _BVB(i)) && (lasttouchedB & _BVB(i)) ) {

      Serial.print(i); Serial.println(" released B");
    }
  }
  // reset our state
  lasttouchedA = currtouchedA;
  lasttouchedB = currtouchedB;


}
