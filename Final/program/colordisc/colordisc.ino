// define pins for in- and out-put
const int ldrs[] = {A0, A1, A2, A3};
const int pushButton = 2;
const int motorControl = 9;

// define LDR data and related variables
int ldrValues[] = {0, 0, 0, 0};
int brightness;
int lightTrack;

// for multi-tasking
unsigned long previousMillis = 0;
const long sensorInterval = 60;

// setting up the RGB Sensor
#include <Wire.h>
#include "Adafruit_TCS34725.h"
// our RGB -> eye-recognized gamma color
byte gammatable[256];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
int colorTrack;

// soundfiles mapped from light & color
int soundTrack;

// for the Music Maker Shield
// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
// pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)
#define CARDCS 4     // Card chip select pin
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin
// new variable from the class (music maker)
Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void setup() {
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  Serial.println("Start");

  // define the pin modes:
  pinMode(pushButton, INPUT);
  pinMode(motorControl, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(ldrs[i], INPUT);
  }

  // initialise the RGB sensor
  if (tcs.begin()) {
    Serial.println("Found RGB sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  Serial.println("after Found RGB sensor");

  // initialise the music maker
  if (! musicPlayer.begin()) {
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  // the SD card on the music maker
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files; commented out to shorten setup time
  // printDirectory(SD.open("/"), 0);

  // Set volume for left, right channels. lower numbers == louder volume
  musicPlayer.setVolume(1, 1);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
}

void loop() {
  // declaring variables for the rgbSensor data
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false); // turn on LED on the rgbSensor

  // read time for multi-tasking, avoid the button being overlooked
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= sensorInterval) {
    previousMillis = currentMillis;

    //////////// brightness ////////////

    // read the ldr data
    for (int i = 0; i < 4; i++) {
      ldrValues[i] = analogRead(ldrs[i]);
    }

    // calculate weighted brightness level, map it to a variable called lightTrack
    brightness = int((ldrValues[0] + ldrValues[1] + ldrValues[2] + ldrValues[3]) / 4); //try different weighting
    if (brightness > 350 ) { 
      lightTrack = 2;
    } else if (brightness > 200 && brightness <= 300) {
      lightTrack = 1;
    } else {
      lightTrack = 0;
    }

    // print out the ldr values
    //Serial.print("ldr1:\t"); Serial.print(ldrValues[0]);
    //Serial.print("\tldr2:\t"); Serial.print(ldrValues[1]);
    //Serial.print("\tldr3:\t"); Serial.print(ldrValues[2]);
    //Serial.print("\tldr4:\t"); Serial.print(ldrValues[3]);
    //Serial.print("\t");
    Serial.print("lightTrack:\t"); Serial.print(lightTrack);
    Serial.print("\t");

    //////////// RGB ////////////

    // read the RGB sensor values
    tcs.getRawData(&red, &green, &blue, &clear);
    //tcs.setInterrupt(true);  // turn off LED

    // Figure out some basic hex code for visualization
    uint32_t sum = clear;
    float r, g, b;
    r = red; r /= sum;
    g = green; g /= sum;
    b = blue; b /= sum;
    r *= 256; g *= 256; b *= 256;

    // calculate the color, map it to a variable called colorTrack
    if (r > 100 && g > 80 && b > 75) {
      colorTrack = 1; // white; non-existed yet
    } else if (r > 100 && g <= 80 && b > 75) {
      colorTrack = 2; // purple
    } else if (r > 100 && g <= 80 && b <= 75) {
      colorTrack = 3; // red or orange
    } else if (r > 100 && g > 80 && b <= 75) {
      colorTrack = 4; // yellow
    } else if (r <= 80 && g > 80 && b > 75) {
      colorTrack = 5; // green
    } else if (r <= 100 && g > 80 && b > 75) {
      colorTrack = 6; // turquois/blue
    } else if (r <= 100 && g <= 80 && b > 75) {
      colorTrack = 7; // very deep blue; non-existed yet
    } else if (r <= 100 && g <= 80 && b <= 75) {
      colorTrack = 8; //black; non-existed yet
    }

    // print out RGB sensor values
        Serial.print("\tr:\t"); Serial.print((int)r);
        Serial.print("\tg:\t"); Serial.print((int)g);
        Serial.print("\tb:\t"); Serial.print((int)b);
        Serial.print("\t");
    //    Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
    Serial.print("\tcolorTrack:\t"); Serial.print(colorTrack);
    Serial.print("\t");


    //////////// Soundtrack ////////////////
    // each combination of a colorTrack and a lightTrack points to a music file, represented by the variable, soundTrack
    soundTrack = colorTrack * 10 + lightTrack;
    Serial.print("soundTrack:\t"); Serial.print(soundTrack);
    Serial.print("\t");

  }


  // read the state of the button and check if it is pressed
  // if it is pressed, turn on the motor and the speaker
  if (digitalRead(pushButton) == HIGH) {
    Serial.print("pushButton on\t");

    // set motor speed
    analogWrite(motorControl, 80); // 255 = full speed

    // play music file based on realtime data sensed by the ldrs and rgbSensor
    switch (soundTrack) {
      case 21:
        musicPlayer.startPlayingFile("track021.mp3");
        break;
      case 22:
        musicPlayer.startPlayingFile("track022.mp3");
        break;
      case 31:
        musicPlayer.startPlayingFile("track031.mp3");
        break;
      case 32:
        musicPlayer.startPlayingFile("track032.mp3");
        break;
      case 41:
        musicPlayer.startPlayingFile("track041.mp3");
        break;
      case 42:
        musicPlayer.startPlayingFile("track042.mp3");
        break;
      case 51:
        musicPlayer.startPlayingFile("track051.mp3");
        break;
      case 52:
        musicPlayer.startPlayingFile("track052.mp3");
        break;
      case 61:
        musicPlayer.startPlayingFile("track061.mp3");
        break;
      case 62:
        musicPlayer.startPlayingFile("track062.mp3");
        break;
//      default:
//        musicPlayer.startPlayingFile("track011.mp3");
    }


  }
  else {
    // if button not pressed
    Serial.print("pushButton off\t");
    analogWrite(motorControl, 0); // stop the motor
  }

  Serial.println();

  // delay in between reads for stability
  delay(100);

}

/// File listing helper
//void printDirectory(File dir, int numTabs) {
//  while (true) {
//
//    File entry =  dir.openNextFile();
//    if (! entry) {
//      // no more files
//      //Serial.println("**nomorefiles**");
//      break;
//    }
//    for (uint8_t i = 0; i < numTabs; i++) {
//      Serial.print('\t');
//    }
//    Serial.print(entry.name());
//    if (entry.isDirectory()) {
//      Serial.println("/");
//      printDirectory(entry, numTabs + 1);
//    } else {
//      // files have sizes, directories do not
//      Serial.print("\t\t");
//      Serial.println(entry.size(), DEC);
//    }
//    entry.close();
//  }
//}
