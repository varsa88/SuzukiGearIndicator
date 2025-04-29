#include <Arduino.h>

/*
  A-TRE

  Input:
  Voltage from bike
  GND
  Output from gear sensor (A0)

  Output:
  Modified output from gear sensor as voltage from PWM (D3)
  Digital outputs controlling sequential number display

  Gear Position Switch Voltages:
  According to forums:
  Neutral 5.00
  1st 1.36
  2nd 1.77
  3rd 2.49
  4th 3.23
  5th 4.10
  6th 4.55 
  Measured on bike with multimeter:
  Neutral 5.14
  1st 1.63 - 1.65 = 1.64
  2nd 1.99 - 2.03 = 2.01
  3rd 2.73 - 2.72 = 2.73
  4th 3.45 - 3.41 = 3.43
  5th 4.30 - 4.27 = 4.28
  6th 4.70
  Measured by arduino:
  1st 1.95
  2nd 2.35
  3rd 3.04
  4th 3.70
  5th 4.37
  6th 4.68
*/
#define UseDisplay
#define UseSerial

#include <movingAvg.h>
#ifdef UseDisplay
#include <SevSeg.h>
#endif

const float gear1st = 1.95;
const float gear2nd = 2.35;
const float gear3rd = 3.04;
const float gear4th = 3.70;
const float gear5th = 4.37;
const float gear6th = 4.68;
const float neutral = 5.0;

const float treGearVoltage = 3.43; // As measured on bike with multimeter.
const float neutralVoltage = neutral;

const float Gear12 = gear1st + ((gear2nd - gear1st) / 2); // 2.15
const float Gear23 = gear2nd + ((gear3rd - gear2nd) / 2); // 2.695
const float Gear34 = gear3rd + ((gear4th - gear3rd) / 2); // 3.37
const float Gear45 = gear4th + ((gear5th - gear4th) / 2); // 4.035
const float Gear56 = gear5th + ((gear6th - gear5th) / 2); // 4.525
const float Gear6n = gear6th + ((neutral - gear6th) / 2); // 4.84

#define GPI_PIN A0
#define PWM_PIN 5

int currentGear;
float outputVoltage;
int outputDuty;

const float VOLTAGE_DIVISOR = 5.0 / 1023.0;
const int PWM_MAX = 255;
const int ADC_MAX = 1023;

#ifdef UseDisplay
SevSeg sevseg; 
#endif
#ifdef UseSerial
String readString;
#endif

movingAvg mySensor(20);

void setup() {
  mySensor.begin();
#ifdef UseSerial
  Serial.begin(9600);
#endif

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

#ifdef UseDisplay
  byte numDigits = 1;   
  byte digitPins[] = {6}; //Digits: 1,2,3,4 <--put one resistor (ex: 220 Ohms, or 330 Ohms, etc, on each digit pin)
  byte segmentPins[] = {11, 10, 12, 13, 4, 7, 8, 9}; //Segments: A,B,C,D,E,F,G,Period
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(10); //Note: 100 brightness simply corresponds to a delay of 2000us after lighting each segment. A brightness of 0 
                            //is a delay of 1us; it doesn't really affect brightness as much as it affects update rate (frequency).
                            //Therefore, for a 4-digit 7-segment + pd, COMMON_ANODE display, the max update rate for a "brightness" of 100 is 1/(2000us*8) = 62.5Hz.
                            //I am choosing a "brightness" of 10 because it increases the max update rate to approx. 1/(200us*8) = 625Hz.
                            //This is preferable, as it decreases aliasing when recording the display with a video camera....I think.
  sevseg.setNumber(8,0);
  sevseg.refreshDisplay(); // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right
  delay(2000);
  sevseg.blank();
  sevseg.refreshDisplay(); // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right
  delay(2000);
#endif
}

void loop() {
  int gpiValueAvg = mySensor.reading(analogRead(GPI_PIN)); // Input sensor value to moving average and return new average
  float inputVoltage = gpiValueAvg * VOLTAGE_DIVISOR;

  if (inputVoltage < Gear12) {currentGear = 1;}
  else if (inputVoltage < Gear23) {currentGear = 2;}
  else if (inputVoltage < Gear34) {currentGear = 3;}
  else if (inputVoltage < Gear45) {currentGear = 4;}
  else if (inputVoltage < Gear56) {currentGear = 5;}
  else if (inputVoltage < Gear6n) {currentGear = 6;}
  else {currentGear = 0;}

  if (currentGear == 0) {
    outputVoltage = neutralVoltage;
#ifdef UseDisplay
    sevseg.setChars("n");
#endif
  } else {
    outputVoltage = treGearVoltage;
#ifdef UseDisplay
    sevseg.setNumber(currentGear);
#endif
  }

  outputDuty = outputVoltage * (PWM_MAX / VOLTAGE_DIVISOR);
  analogWrite(PWM_PIN, outputDuty);

#ifdef UseSerial
  // send data only when you receive data:
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() > 0) {
    Serial.println(readString);  //so you can see the captured string
    int n = readString.toInt();  //convert readString into a number
    readString=""; //empty for next input
    // Serial.print("I received: ");
    // Serial.println(n, DEC);
    Serial.print("inputVoltage: ");
    Serial.println(inputVoltage);
    Serial.print("currentGear: ");
    Serial.println(currentGear);
    Serial.print("outputVoltage: ");
    Serial.println(outputVoltage);
  }
#endif
#ifdef UseDisplay
  sevseg.refreshDisplay(); // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right
#endif
}
