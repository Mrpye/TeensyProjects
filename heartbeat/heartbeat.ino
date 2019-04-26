#include "Adafruit_MPR121.h"
#include <Audio.h>
#include <Bounce.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>

#define SDCARD_CS_PIN BUILTIN_SDCARD
#define SDCARD_MOSI_PIN 7
#define SDCARD_SCK_PIN 14

#ifndef _BVA
#define _BVA(bit) (1 << (bit))
#endif
#ifndef _BVB
#define _BVB(bit) (1 << (bit))
#endif

// GUItool: begin automatically generated code
AudioPlaySerialflashRaw player[] = {
    AudioPlaySerialflashRaw(), AudioPlaySerialflashRaw(),
    AudioPlaySerialflashRaw(), AudioPlaySerialflashRaw(),
    AudioPlaySerialflashRaw(), AudioPlaySerialflashRaw(),
    AudioPlaySerialflashRaw(), AudioPlaySerialflashRaw()};

AudioMixer4 mixer[] = {AudioMixer4(), AudioMixer4(), AudioMixer4()};

AudioOutputI2S i2s2; // xy=1185,270

AudioConnection patchCord1(player[0], 0, mixer[0], 0);
AudioConnection patchCord2(player[1], 0, mixer[0], 1);
AudioConnection patchCord3(player[2], 0, mixer[0], 2);
AudioConnection patchCord4(player[3], 0, mixer[0], 3);

AudioConnection patchCord5(player[4], 0, mixer[1], 0);
AudioConnection patchCord6(player[5], 0, mixer[1], 1);
AudioConnection patchCord7(player[6], 0, mixer[1], 2);
AudioConnection patchCord8(player[7], 0, mixer[1], 3);

AudioConnection patchCord9(mixer[0], 0, mixer[2], 0);
AudioConnection patchCord10(mixer[1], 0, mixer[2], 1);

AudioConnection patchCord11(mixer[2], 0, i2s2, 0);
AudioConnection patchCord12(mixer[2], 0, i2s2, 1);

AudioControlSGTL5000 sgtl5000_1; // xy=1176,432
// GUItool: end automatically generated code

String sounds[] = {"KICK.RAW",   "SNARE.RAW", "CLAP.RAW", "HIHAT.RAW",
                   "CHIME3.TRW", "KISS.RAW",  "B1.TRW",   "C2.TRW"};
int pattern[8][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

int pattern_index = 0;
int current_sound = 0;
int tempo = 100;
int last_tempo = 100;
static uint32_t next;
// Flash file
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
const int SDchipSelect = 10;   // Audio Shield has SD card CS on pin 10
SerialFlashFile file;
const unsigned long testIncrement = 4096;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 capA = Adafruit_MPR121();
Adafruit_MPR121 capB = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouchedA = 0;
uint16_t currtouchedA = 0;
uint16_t lasttouchedB = 0;
uint16_t currtouchedB = 0;

const int buttonFill = 2;
const int buttonPlay = 3;
const int buttonBankUp = 5;
const int buttonBankDown = 4;

bool isplaying = true;

Bounce playbutton = Bounce(buttonPlay, 10);     // 10 ms debounce
Bounce fillbutton = Bounce(buttonFill, 10);     // 10 ms debounce
Bounce upbutton = Bounce(buttonBankUp, 10);     // 10 ms debounce
Bounce downbutton = Bounce(buttonBankDown, 10); // 10 ms debounce

char *string2char(String str);
void PlaySound(int i);
float eraseBytesPerSecond(const unsigned char *id);
void error(const char *message);
void erase_flash();
void flash_memeory();
bool compareFiles(File &file, SerialFlashFile &ffile);

void setup() {

  // Serial.begin(9600);
  Serial.begin(9600);
  delay(1000);

  // wait up to 10 seconds for Arduino Serial Monitor
  unsigned long startMillis = millis();
  while (!Serial && (millis() - startMillis < 10000))
    ;
  delay(100);

  Serial.println("Adafruit MPR121 Capacitive Touch sensor A test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!capA.begin(0x5A)) {
    Serial.println("MPR121 A not found, check wiring?");
    while (1)
      ;
  }
  Serial.println("MPR121 A found!");

  // Default address is 0x5B, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5B and if SCL then 0x5D
  if (!capB.begin(0x5B)) {
    Serial.println("MPR121 B not found, check wiring?");
    while (1)
      ;
  }

  Serial.println("MPR121 B found!");

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  SPI.setSCK(14); // Audio shield has SCK on pin 14
  SPI.setMISO(12);
  SPI.setMOSI(7); // Audio shield has MOSI on pin 7

  if (!SerialFlash.begin(FlashChipSelect)) {
    Serial.println("Cannot find chip");
    while (1)
      ;
  }

  //*****************
  // Setup the buttons
  //*****************
  pinMode(buttonPlay, INPUT_PULLUP);
  pinMode(buttonFill, INPUT_PULLUP);
  pinMode(buttonBankUp, INPUT_PULLUP);
  pinMode(buttonBankDown, INPUT_PULLUP);

  int flash = digitalRead(buttonPlay);
  if (flash == LOW) {
    Serial.println("Flashing Memory");
    erase_flash();
    delay(1000);
    flash_memeory();
    Serial.println("Finished Flashing Memory!!");
  }

  //*****************
  // Setup the volume
  //*****************
  sgtl5000_1.enable();
  sgtl5000_1.volume(1.0);
  const float vol = 0.35;

  for (int i = 0; i < 4; i++) {
    mixer[0].gain(i, vol);
    mixer[1].gain(i, vol);
    mixer[2].gain(i, vol);
  }
}

void loop() {

  playbutton.update();
  fillbutton.update();
  upbutton.update();
  downbutton.update(); //

  if (playbutton.risingEdge()) {
    Serial.println("Play 1 pressed");
    isplaying = !isplaying;
    if (isplaying == true) {
      pattern_index = 0;
      Serial.println("Playing");
    }
  }
  if (fillbutton.risingEdge()) {
    Serial.println("fill 1 pressed");
  }
  if (upbutton.fallingEdge()) {
    Serial.println("upbutton 1 pressed");
  }
  if (downbutton.fallingEdge()) {
    Serial.println("downbutton 1 pressed");
  }

  int tempA = analogRead(A6);
  tempo = map(tempA, 0, 1023, 300, 50);
  if (tempo != last_tempo) {
    Serial.print("Temp ");
    Serial.println(tempo);
  }
  last_tempo = tempo;
  currtouchedA = capA.touched();
  currtouchedB = capB.touched();

  for (uint8_t i = 0; i < 13; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouchedA & _BVA(i)) && !(lasttouchedA & _BVA(i))) {
      pattern[current_sound][i] =
          !pattern[current_sound][i]; // set the current pattern
      Serial.print("Patter index");
      Serial.println(i);
    }
  }

  for (uint8_t i = 0; i < 13; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouchedB & _BVB(i)) && !(lasttouchedB & _BVB(i))) {
      if (i < 4) {
        pattern[current_sound][i + 12] =
            !pattern[current_sound][i + 12]; // set the current pattern
        Serial.print("Patter index");
        Serial.println(i + 12);
      } else {
        current_sound = i - 4;
        Serial.print("current sound");
        Serial.println(current_sound);
      }
    }
  }
  // reset our state
  lasttouchedA = currtouchedA;
  lasttouchedB = currtouchedB;

  if (millis() >= next) {
    next = millis() + tempo;

    for (int i = 0; i < 8; i++) {
      if (pattern[i][pattern_index] == 1) {
        if (isplaying == true) {
          PlaySound(i);
        }
      }
    }
    pattern_index = pattern_index + 1;
    if (pattern_index > 15) {
      pattern_index = 0;
    }
  }
}

char *string2char(String str) {
  if (str.length() != 0) {
    char *p = const_cast<char *>(str.c_str());
    return p;
  }
}

float eraseBytesPerSecond(const unsigned char *id) {
  if (id[0] == 0x20)
    return 152000.0; // Micron
  if (id[0] == 0x01)
    return 500000.0; // Spansion
  if (id[0] == 0xEF)
    return 419430.0; // Winbond
  if (id[0] == 0xC2)
    return 279620.0; // Macronix
  return 320000.0;   // guess?
}

void PlaySound(int i) {
  if (!player[i].isPlaying()) {
    player[i].stop();
    player[i].play(string2char(sounds[i]));
  } else {
    player[i].play(string2char(sounds[i]));
  }
}

void flash_memeory() {
  Serial.println("Copy all files from SD Card to SPI Flash");

  if (!SD.begin(SDchipSelect)) {
    error("Unable to access SD card");
  }
  if (!SerialFlash.begin(FlashChipSelect)) {
    error("Unable to access SPI Flash chip");
  }

  File rootdir = SD.open("/");
  while (1) {
    // open a file from the SD card
    Serial.println();
    File f = rootdir.openNextFile();
    if (!f)
      break;
    const char *filename = f.name();
    Serial.print(filename);
    Serial.print("    ");
    unsigned long length = f.size();
    Serial.println(length);

    // check if this file is already on the Flash chip
    if (SerialFlash.exists(filename)) {
      Serial.println("  already exists on the Flash chip");
      SerialFlashFile ff = SerialFlash.open(filename);
      if (ff && ff.size() == f.size()) {
        Serial.println("  size is the same, comparing data...");
        if (compareFiles(f, ff) == true) {
          Serial.println("  files are identical :)");
          f.close();
          ff.close();
          continue; // advance to next file
        } else {
          Serial.println("  files are different");
        }
      } else {
        Serial.print("  size is different, ");
        Serial.print(ff.size());
        Serial.println(" bytes");
      }
      // delete the copy on the Flash chip, if different
      Serial.println("  delete file from Flash chip");
      SerialFlash.remove(filename);
    }

    // create the file on the Flash chip and copy data
    if (SerialFlash.create(filename, length)) {
      SerialFlashFile ff = SerialFlash.open(filename);
      if (ff) {
        Serial.print("  copying");
        // copy data loop
        unsigned long count = 0;
        unsigned char dotcount = 9;
        while (count < length) {
          char buf[256];
          unsigned int n;
          n = f.read(buf, 256);
          ff.write(buf, n);
          count = count + n;
          Serial.print(".");
          if (++dotcount > 100) {
            Serial.println();
            dotcount = 0;
          }
        }
        ff.close();
        if (dotcount > 0)
          Serial.println();
      } else {
        Serial.println("  error opening freshly created file!");
      }
    } else {
      Serial.println("  unable to create file");
    }
    f.close();
  }
  rootdir.close();
  delay(10);
  Serial.println("Finished All Files");
}
void erase_flash() {
  unsigned long startMillis = millis();
  delay(100);

  if (!SerialFlash.begin(FlashChipSelect)) {
    while (1) {
      Serial.println("Unable to access SPI Flash chip");
      delay(1000);
    }
  }
  unsigned char id[5];
  SerialFlash.readID(id);
  unsigned long size = SerialFlash.capacity(id);

  if (size > 0) {
    Serial.print("Flash Memory has ");
    Serial.print(size);
    Serial.println(" bytes.");
    Serial.println("Erasing ALL Flash Memory:");
    // Estimate the (lengthy) wait time.
    Serial.print("  estimated wait: ");
    int seconds = (float)size / eraseBytesPerSecond(id) + 0.5;
    Serial.print(seconds);
    Serial.println(" seconds.");
    Serial.println("  Yes, full chip erase is SLOW!");
    SerialFlash.eraseAll();
    unsigned long dotMillis = millis();
    unsigned char dotcount = 0;
    while (SerialFlash.ready() == false) {
      if (millis() - dotMillis > 1000) {
        dotMillis = dotMillis + 1000;
        Serial.print(".");
        dotcount = dotcount + 1;
        if (dotcount >= 60) {
          Serial.println();
          dotcount = 0;
        }
      }
    }
    if (dotcount > 0)
      Serial.println();
    Serial.println("Erase completed");
    unsigned long elapsed = millis() - startMillis;
    Serial.print("  actual wait: ");
    Serial.print(elapsed / 1000ul);
    Serial.println(" seconds.");
  }
}
bool compareFiles(File &file, SerialFlashFile &ffile) {
  file.seek(0);
  ffile.seek(0);
  unsigned long count = file.size();
  while (count > 0) {
    char buf1[128], buf2[128];
    unsigned long n = count;
    if (n > 128)
      n = 128;
    file.read(buf1, n);
    ffile.read(buf2, n);
    if (memcmp(buf1, buf2, n) != 0)
      return false; // differ
    count = count - n;
  }
  return true; // all data identical
}
void error(const char *message) {
  while (1) {
    Serial.println(message);
    delay(2500);
  }
}
