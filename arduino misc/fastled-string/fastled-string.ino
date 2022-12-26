
// #define FASTLED_ALLOW_INTERRUPTS 0
// #define FASTLED_INTERRUPT_RETRY_COUNT 1
// #define FORCE_SOFTWARE_SPI 1
// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1


#include <FastLED.h>

#include <SoftwareSerial.h>

#define MAX_POWER_MILLIAMPS 500
#define LED_TYPE            WS2812
#define COLOR_ORDER         GRB

#define FPS 60

#define NUM_LEDS_DATA 26

#define NUM_LEDS_1 14
#define NUM_LEDS_2 6
#define NUM_LEDS_3 6


#define DATA_PIN_1 10
#define DATA_PIN_2 3
#define DATA_PIN_3 5
// #define DATA_PIN_1 ~D10
// #define DATA_PIN_2 ~D11
// #define DATA_PIN_3 D12
// #define DATA_PIN_1 D10
// #define DATA_PIN_2 D11
// #define DATA_PIN_3 D12

CRGB leds1[NUM_LEDS_1];
CRGB leds2[NUM_LEDS_2];
CRGB leds3[NUM_LEDS_3];

int refreshTime = 1000 / FPS;

void setup() {
  // initialize Serial communication at 9600 bits per second:
  // delay(1000);
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  pinMode(DATA_PIN_1, OUTPUT);
  pinMode(DATA_PIN_2, OUTPUT);
  pinMode(DATA_PIN_3, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  FastLED.clear();
  FastLED.clearData();

  FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds1,NUM_LEDS_DATA);
  FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds2,NUM_LEDS_DATA);
  FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds3,NUM_LEDS_DATA);

    FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);

    FastLED.setMaxRefreshRate(400);

    // FastLED.setDither( 0 );

}

bool shown = false;
int compression = 4;

int saturationBuffer = 0;
int colorForeground = 0;
int colorMidground = 0;
int colorBackground = 0;
int brightnessBackground = 0;
int brightnessForeground = 0;

int buffer00 = 0;
int buffer01 = 0;
int buffer02 = 0;
int buffer03 = 0;
int buffer04 = 0;
int buffer05 = 0;

int buffer30 = 0;
int buffer31 = 0;
int buffer32 = 0;
int buffer33 = 0;
int buffer34 = 0;
int buffer35 = 0;

int buffer40 = 0;
int buffer41 = 0;
int buffer42 = 0;
int buffer43 = 0;
int buffer44 = 0;
int buffer45 = 0;

int buffer50 = 0;
int buffer51 = 0;
int buffer52 = 0;
int buffer53 = 0;
int buffer54 = 0;
int buffer55 = 0;

int buffer60 = 0;
int buffer61 = 0;
int buffer62 = 0;
int buffer63 = 0;
int buffer64 = 0;
int buffer65 = 0;

int buffer70 = 0;
int buffer71 = 0;
int buffer72 = 0;
int buffer73 = 0;
int buffer74 = 0;
int buffer75 = 0;

bool initialSet = false;

int Compress(int value, int max = 255) {
  if (compression == 0 || compression == 1) { return value; }
  return map(value, 0, max, 0, max / compression) * compression;
}

// the loop routine runs over and over again forever:
void loop() {

  // if (!initialized) { return; }
  // FastLED.clear();
  // for (int i = 1; i <= 3; i++) {
  //   SetStripColor(i,255,200,69);
  // }


  int s0 = analogRead(A0);    // saturation
  // int s1 = analogRead(A1); // SKIP
  // int s2 = analogRead(A2); // SKIP
  int s3 = analogRead(A3);    // color 1 (foreground)
  int s4 = analogRead(A4);    // color 2 (midground)
  int s5 = analogRead(A5);    // color 3 (background)
  int s6 = analogRead(A6);    // brightness (background) 
  int s7 = analogRead(A7);    // brightness (fore/midground)

  s0 = Compress(s0, 1023);
  s3 = Compress(s3, 1023);
  s4 = Compress(s4, 1023);
  s5 = Compress(s5, 1023);
  s6 = Compress(s6, 1023);
  s7 = Compress(s7, 1023);

  AddBuffer(0, s0);
  AddBuffer(3, s3);
  AddBuffer(4, s4);
  AddBuffer(5, s5);
  AddBuffer(6, s6);
  AddBuffer(7, s7);
  InitialBufferSet();

  int saturation =           map(GetBuffer(0), 0, 1023, 255, 0); // saturation
  int colorForeground =      map(GetBuffer(3), 0, 1023, 0, 255); // color 1 (foreground)
  int colorMidground =       map(GetBuffer(4), 0, 1023, 0, 255); // color 2 (midground)
  int colorBackground =      map(GetBuffer(5), 0, 1023, 0, 255); // color 3 (background)
  int brightnessBackground = map(GetBuffer(6), 0, 1023, 0, 255); // brightness (background) 
  int brightnessForeground = map(GetBuffer(7), 0, 1023, 0, 255); // brightness (fore/midground)

  saturation = Compress(saturation);
  colorForeground = Compress(colorForeground);
  colorMidground = Compress(colorMidground);
  colorBackground = Compress(colorBackground);
  brightnessBackground = Compress(brightnessBackground);
  brightnessForeground = Compress(brightnessForeground);

  Serial.print("saturation: ");Serial.print(saturation);Serial.print(",");
  Serial.print("colorForeground: ");Serial.print(colorForeground);Serial.print(",");
  Serial.print("colorMidground: ");Serial.print(colorMidground);Serial.print(",");
  Serial.print("colorBackground: ");Serial.print(colorBackground);Serial.print(",");
  Serial.print("brightnessBackground: ");Serial.print(brightnessBackground);Serial.print(",");
  Serial.print("brightnessForeground: ");Serial.print(brightnessForeground);Serial.println("");

  SetStripHSV(1, colorForeground, saturation, brightnessForeground);
  SetStripHSV(2, colorMidground,  saturation, brightnessForeground);
  SetStripHSV(3, colorBackground, saturation, brightnessBackground);

  // if (!shown) {
  //   shown = true;
    FastLED.show();
  // }

  // FastLED.delay(refreshTime);
  // FastLED.delay(10);
  FastLED.delay(refreshTime);
}

int GetNum(int strip) {
  switch (strip) {
    case 1:
      return NUM_LEDS_1;
      case 2:
      return NUM_LEDS_2;
      case 3:
      return NUM_LEDS_2;
  }
  return 0;
}
void SetStripHSV(int strip, int h, int s, int v) {
  int num = GetNum(strip);
  for (int i = 0; i < num; i++) {
    switch (strip) { 
      case 1: leds1[i].setHSV(h, s, v); break;
      case 2: leds2[i].setHSV(h, s, v); break;
      case 3: leds3[i].setHSV(h, s, v); break;
    }
  }
}
void SetStripRGB(int strip, byte r, byte g, byte b) {
  int num = GetNum(strip);
  for (int i = 0; i < num; i++) {
    switch (strip) { 
      case 1: leds1[i].setRGB(r,g,b); break;
      case 2: leds2[i].setRGB(r,g,b); break;
      case 3: leds3[i].setRGB(r,g,b); break;
    }
  }
}

void AddBuffer(int buffer, int value) {
  switch (buffer) {
    case 0:
      buffer05 = 
      buffer04;
      buffer04 = 
      buffer03;
      buffer03 = 
      buffer02;
      buffer02 = 
      buffer01;
      buffer01 = 
      buffer00;
      buffer00 = value; break;
    case 3:
      buffer35 = 
      buffer34;
      buffer34 = 
      buffer33;
      buffer33 = 
      buffer32;
      buffer32 = 
      buffer31;
      buffer31 = 
      buffer30;
      buffer30 = value; break;
    case 4:
      buffer45 = 
      buffer44;
      buffer44 = 
      buffer43;
      buffer43 = 
      buffer42;
      buffer42 = 
      buffer41;
      buffer41 = 
      buffer40;
      buffer40 = value; break;
    case 5:
      buffer55 = 
      buffer54;
      buffer54 = 
      buffer53;
      buffer53 = 
      buffer52;
      buffer52 = 
      buffer51;
      buffer51 = 
      buffer50;
      buffer50 = value; break;
    case 6:
      buffer65 = 
      buffer64;
      buffer64 = 
      buffer63;
      buffer63 = 
      buffer62;
      buffer62 = 
      buffer61;
      buffer61 = 
      buffer60;
      buffer60 = value; break;
    case 7:
      buffer75 = 
      buffer74;
      buffer74 = 
      buffer73;
      buffer73 = 
      buffer72;
      buffer72 = 
      buffer71;
      buffer71 = 
      buffer70;
      buffer70 = value; break;
  }
}

int GetBuffer(int buffer) {
  switch (buffer) {
    case 0:
      return (
        buffer00 + 
        buffer01 + 
        buffer02 +
        buffer03 +
        buffer04 +
        buffer05) / 6;
    case 3:
      return (
        buffer30 + 
        buffer31 + 
        buffer32 +
        buffer33 +
        buffer34 +
        buffer35) / 6;
    case 4:
      return (
        buffer40 + 
        buffer41 + 
        buffer42 +
        buffer43 +
        buffer44 +
        buffer45) / 6;
    case 5:
      return (
        buffer50 + 
        buffer51 + 
        buffer52 +
        buffer53 +
        buffer54 +
        buffer55) / 6;
    case 6:
      return (
        buffer60 + 
        buffer61 + 
        buffer62 +
        buffer63 +
        buffer64 +
        buffer65) / 6;
    case 7:
      return (
        buffer70 + 
        buffer71 + 
        buffer72 +
        buffer73 +
        buffer74 +
        buffer75) / 6;
  }
  return 0;
}

void InitialBufferSet() {
  if (initialSet) {return;}
  initialSet = true;
  // initial prep of buffers 
  buffer01 = buffer00;
  buffer02 = buffer00;
  buffer03 = buffer00;
  buffer04 = buffer00;
  buffer05 = buffer00;
  buffer31 = buffer30;
  buffer32 = buffer30;
  buffer33 = buffer30;
  buffer34 = buffer30;
  buffer35 = buffer30;
  buffer41 = buffer40;
  buffer42 = buffer40;
  buffer43 = buffer40;
  buffer44 = buffer40;
  buffer45 = buffer40;
  buffer51 = buffer50;
  buffer52 = buffer50;
  buffer53 = buffer50;
  buffer54 = buffer50;
  buffer55 = buffer50;
  buffer61 = buffer60;
  buffer62 = buffer60;
  buffer63 = buffer60;
  buffer64 = buffer60;
  buffer65 = buffer60;
  buffer71 = buffer70;
  buffer72 = buffer70;
  buffer73 = buffer70;
  buffer74 = buffer70;
  buffer75 = buffer70;

}


