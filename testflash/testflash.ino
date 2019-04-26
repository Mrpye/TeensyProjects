#include <Audio.h>
#include <SerialFlash.h>

AudioPlaySerialflashRaw  sound0;
AudioMixer4        mix;
AudioOutputI2S     headphones;

AudioConnection patchCord1( sound0, 0, mix, 0 );
AudioConnection patchCord2( mix, 0, headphones, 0 );
AudioConnection patchCord3( mix, 0, headphones, 1 );

AudioControlSGTL5000     sgtl5000;


void setup() 
  {
  Serial.begin( 9600 );

  // wait up to 10 seconds for Arduino Serial Monitor
  unsigned long startMillis = millis();
  while ( !Serial && ( millis() - startMillis < 10000 ) );

  // Audio connections require memory to work
  AudioMemory( 8 );

  SPI.setMOSI( 7 );   // MOSI_PIN for audio board
  SPI.setSCK( 14 );   // SCK_PIN for audio board

  if ( !SerialFlash.begin() )
    Serial.println( "Unable to access SPI Flash chip" );

  sgtl5000.enable();
  sgtl5000.volume( 0.5 );
  }


void playFile( const char* filename )
  {
  Serial.print( "Playing file: " );
  Serial.print( filename );

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  sound0.play( filename );

  // A brief delay for the library read info
  delay( 5 );

  // Simply wait for the file to finish playing.
  while ( sound0.isPlaying() );

  Serial.println( "       DONE" );
  }


void loop() 
  {
  playFile( "C1.TRW" );

  delay( 5000 );
  }

/*#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySerialflashRaw  playFlashRaw1;  //xy=599,124
AudioMixer4              mixer1;         //xy=790,133
AudioOutputI2S           i2s2;           //xy=944,134
AudioConnection          patchCord1(playFlashRaw1, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, 0, i2s2, 0);
AudioConnection          patchCord3(mixer1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=908,209
// GUItool: end automatically generated code

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

const int FlashChipSelect = 6; // digital pin for flash chip CS pin

void error(const char *message) {
  while (1) {
    Serial.println(message);
    delay(2500);
  }
}
void setup() {
 //uncomment these if using Teensy audio shield
  SPI.setSCK(14);  // Audio shield has SCK on pin 14
  SPI.setMOSI(7);  // Audio shield has MOSI on pin 7

  //uncomment these if you have other SPI chips connected
  //to keep them disabled while using only SerialFlash
  //pinMode(4, INPUT_PULLUP);
  //pinMode(10, INPUT_PULLUP);

  Serial.begin(9600);

  // wait up to 10 seconds for Arduino Serial Monitor
  unsigned long startMillis = millis();
  while (!Serial && (millis() - startMillis < 10000)) ;
  delay(100);

  if (!SerialFlash.begin(FlashChipSelect)) {
    while (1) {
      Serial.println("Unable to access SPI Flash chip");
      delay(1000);
    }
  }
  
  
  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
   mixer1.gain(0, 0.5);
Serial.print("hello");
//playFlashRaw1.play("C1.TRW");
}

void loop() {
  // put your main code here, to run repeatedly:
if(!playFlashRaw1.isPlaying()){
  Serial.print("hello");
playFlashRaw1.play("C1.TRW");

}
}*/
