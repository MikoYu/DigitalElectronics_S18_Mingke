//analogRead of photocells
int ldr1 = A0;
int ldr2 = A1;
int ldr3 = A2;
int ldr4 = A3;

//setting up rgbSensor
#include <Wire.h>
#include "Adafruit_TCS34725.h"
// our RGB -> eye-recognized gamma color
byte gammatable[256];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// give a name to digital pin 2, which has a pushbutton attached
int pushButton = 2;

// speaker: notes to play
#include "pitches.h"
int speakerControl = 8;
int note = NOTE_A3;

// the transistor which controls the motor will be attached to digital pin 9
int motorControl = 9;

/*
  //if use an LED as an indicator
  // Pick analog outputs, for the UNO these three work well
  // use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
  #define redpin 3
  #define greenpin 6
  #define bluepin 5
  // for a common anode LED, connect the common pin to +5V
  // for common cathode, connect the common to ground

  // set to false if using a common cathode LED
  #define commonAnode false
*/

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  Serial.println("Start");

  // define the pin modes:
  pinMode(pushButton, INPUT);
  pinMode(motorControl, OUTPUT);
  pinMode(speakerControl, OUTPUT);


  if (tcs.begin()) {
    Serial.println("Found RGB sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  Serial.println("after Found RGB sensor");
  /*
    // use these three pins to drive an LED
    pinMode(redpin, OUTPUT);
    pinMode(greenpin, OUTPUT);
    pinMode(bluepin, OUTPUT);

    // thanks PhilB for this gamma table!
    // it helps convert RGB colors to what humans see
    for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
    }
  */

}

void loop() {
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read

  // read the ldr data
  int ldrValue1 = analogRead(ldr1);
  int ldrValue2 = analogRead(ldr2);
  int ldrValue3 = analogRead(ldr3);
  int ldrValue4 = analogRead(ldr4);

  // print out the value you read:
  Serial.print("ldr1:\t"); Serial.print(ldrValue1);
  Serial.print("\tldr2:\t"); Serial.print(ldrValue2);
  Serial.print("\tldr3:\t"); Serial.print(ldrValue3);
  Serial.print("\tldr4:\t"); Serial.print(ldrValue4);
  //Serial.println();

  delay(60);        // delay in between reads for stability

  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED

  Serial.print("\t");
  Serial.print("\tC:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.print(blue);

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  Serial.print("\t");
  Serial.print("\tr:\t"); Serial.print((int)r);
  Serial.print("\tg:\t"); Serial.print((int)g);
  Serial.print("\tb:\t"); Serial.print((int)b);

  Serial.print("\t");
  Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);

  Serial.print("\t");


  //Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );

  /*
    analogWrite(redpin, gammatable[(int)r]);
    analogWrite(greenpin, gammatable[(int)g]);
    analogWrite(bluepin, gammatable[(int)b]);
  */

  //mapping light to notes
  note = (ldrValue1 + 2 * ldrValue2 + 3 * ldrValue3 + 4 * ldrValue4) / 10;
  //  note = r * g * b * note / 10000;
  //  Serial.print(int(note) + "/t");


  // read the state of the button and check if it is pressed
  if (digitalRead(pushButton) == HIGH) {
    Serial.print("pushButton on\t");
    analogWrite(motorControl, 127); //127 = half speed (255 = full)
    tone(speakerControl, note);
  }
  else {
    Serial.print("pushButton off\t");
    analogWrite(motorControl, 0);
    noTone(speakerControl);
  }

  Serial.println();

  delay(1);        // delay in between reads for stability

}
