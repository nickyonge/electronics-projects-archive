#include <heartRate.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>

/*
  Optical SP02 Detection (SPK Algorithm) using the MAX30105 Breakout
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 19th, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  This demo shows heart rate and SPO2 levels.

  It is best to attach the sensor to your finger using a rubber band or other tightening 
  device. Humans are generally bad at applying constant pressure to a thing. When you 
  press your finger against the sensor it varies enough to cause the blood in your 
  finger to flow differently which causes the sensor readings to go wonky.

  This example is based on MAXREFDES117 and RD117_LILYPAD.ino from Maxim. Their example
  was modified to work with the SparkFun MAX30105 library and to compile under Arduino 1.6.11
  Please see license file for more info.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected
 
  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/

#include <Wire.h>

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

byte pulseLED = 11; //Must be on PWM pin
byte readLED = 13; //Blinks with each data read

//byte pinR = 2;//a2 d16
//byte pinG = 6;//a6 d20
//byte pinB = 7;//a7 d21
byte pinR = 4;//16;//a2 d16
byte pinG = 12;//a6 d20
byte pinB = 12;//a7 d21

byte samplesCollected = 0;
int8_t highestSample = 0;

void setup()
{
  Serial.begin(115200); // initialize serial communication at 115200 bits per second:

  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  pinMode(10, OUTPUT);pinMode(3, OUTPUT);pinMode(2,OUTPUT);
  digitalWrite(10,HIGH);digitalWrite(2,HIGH);digitalWrite(3,HIGH);

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
    SetColor(0);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

//  Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));
  
//  while (Serial.available() == 0) ; //wait until user presses a key
  Serial.read();

  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  SetColor(1);

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void SetOx(int32_t level32) {
  int level = (int)level32;
  if (level >= 96) {
    SetColor(2);
  } else if (level >= 94) {
    SetColor(3);
  } else if (level >= 90) {
    SetColor(4);
  } else if (level >= 50) {
    SetColor(5);
  } else {
    SetColor(-1);
  }
}

bool r = false;
bool g = false;
bool b = false;

void SetColor(int color) {
  
  r = false;
  g = false;
  b = false;
  
  switch (color) {
    case 0:
    // magenta - no finger
    r = true;
    b = true;
    break;
    case 1:
    // white - reading 
    r = true;
    b = true;
    g = true;
    break;
    case 2:
    // blue - v good 96-100
    b = true;
    break;
    case 3:
    // green - alright 94-96
    g = true;
    break;
    case 4:
    // yellow - iffy 90-94
    r = true;
    g = true;
    break;
    case 5:
    // red - bad under 89
    r = true;
    break;
    case -1:
    // bad read 
    // off
    break;
    
    case 10:
    r = true;
    //red 
    break;
    
    case 11:
    b = true;
    // blue
    break;
    
    case 12:
    g = true;
    // green 
    break;
  }
  UpdateLights();
}

void loop() {
//  loopRGB();
  pinMode(A6, OUTPUT);
  digitalWrite(A6, LOW);
}

void L1() {
  digitalWrite(pinR, HIGH);
  digitalWrite(pinG, HIGH);
  digitalWrite(pinB, HIGH);
}

void UpdateLights() {
  analogWrite(pinR, r ? 100 : 0);
  analogWrite(pinG, g ? 100 : 0);
  analogWrite(pinB, b ? 100 : 0);
//  digitalWrite(pinR, !r ? HIGH : LOW);
//  digitalWrite(pinG, !g ? HIGH : LOW);
//  digitalWrite(pinB, !b ? HIGH : LOW);
//  Serial.print(F("\n  RGB="));
//  Serial.print(r);
//  Serial.print(g);
//  Serial.print(b);
}


void loopRGB() {
  SetColor(10);
  delay(333);
  SetColor(11);
  delay(333);
  SetColor(12);
  delay(333);
  SetColor(-1);
  delay(333);
  r = true;
  g = true;
  UpdateLights();
  delay(333);
  g = false;
  b = true;
  UpdateLights();
  delay(333);
  r = false;
  g = true;
  UpdateLights();
  delay(333);
  SetColor(-1);
  delay(333);
  SetColor(1);
  delay(333);
}


void cycleAll() {
  r = false;
  g = false;
  b = false;
  UpdateLights(); delay(400);
  r = true;
  UpdateLights(); delay(400);
  g = true;
  UpdateLights(); delay(400);
  b = true;
  UpdateLights(); delay(400);
  r = false;
  UpdateLights(); delay(400);
  g = false;
  UpdateLights(); delay(400);
  r = true;
  UpdateLights(); delay(400);
  r = false; g = false; b = false;
  UpdateLights(); delay(200);
  r = true;UpdateLights(); delay(200);
  r = false;g = true;UpdateLights(); delay(200);
  g = false; b = true; UpdateLights(); delay(200);
  r = true; g = true; b = false; UpdateLights(); delay(200);
  r = true; g = false; b = true; UpdateLights(); delay(200);
  r = false; g = true; UpdateLights(); delay(200);
  r = true; UpdateLights(); delay(1000);
}

int i = -1;
void loopStates() {
  SetColor(i);
  delay(250);
  i++;
  if (i > 5) { i = -1;}
}

void loopA() {
  SetColor(10);
}

void loopB()
{
  UpdateLights();

  
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sample

    Serial.print(F("red="));
    Serial.print(redBuffer[i], DEC);
    Serial.print(F(", ir="));
    Serial.println(irBuffer[i], DEC);
  }

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    
UpdateLights();
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every data read

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      //send samples and calculation result to terminal program through UART
      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.print(irBuffer[i], DEC);

      Serial.print(F(", HR="));
      Serial.print(heartRate, DEC);

      Serial.print(F(", HRvalid="));
      Serial.print(validHeartRate, DEC);

      Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);

      Serial.print(F(", SPO2Valid="));
      Serial.println(validSPO2, DEC);

    }

    //After gathering 25 new samples recalculate HR and SP02

    bool valid = validSPO2 > 0;

    if (valid && spo2 > 70 && spo2 < 100) {

      if (samplesCollected < 10) { samplesCollected++; }

      if (spo2 > highestSample) {
        highestSample = spo2;
      }

    }
    
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);


    if (valid && samplesCollected > 2) {
      
      SetOx(highestSample);
      
    } else {
      SetColor(1);
    }

    
  }

  
}
