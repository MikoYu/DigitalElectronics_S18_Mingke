/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

int ldr1 = A0;
int ldr2 = A1;
int ldr3 = A2;
int ldr4 = A3;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0 - 3:
  int ldrValue1 = analogRead(ldr1);
  int ldrValue2 = analogRead(ldr2);
  int ldrValue3 = analogRead(ldr3);
  int ldrValue4 = analogRead(ldr4);

  // print out the value you read:
  Serial.print("ldr1:\t"); Serial.print(ldrValue1);
  Serial.print("\tldr2:\t"); Serial.print(ldrValue2);
  Serial.print("\tldr3:\t"); Serial.print(ldrValue3);
  Serial.print("\tldr4:\t"); Serial.print(ldrValue4);
  Serial.println();


  delay(60);        // delay in between reads for stability
}
