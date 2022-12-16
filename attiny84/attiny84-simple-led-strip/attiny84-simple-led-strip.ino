#include <FastLED.h>

#define LED_NUM 15
#define LED_PIN 0

CRGB leds[LED_NUM];

const byte LED_LIT_COUNT = 10;

const byte DELAY_MS = 5;
const byte LIT_INCREMENT = 8;
const byte HUE_BASE_INCREMENT = 1;
const byte HUE_INDEX_INCREMENT = 18;
const byte BRIGHTNESS_INCREMENT = 6;


const byte SATURATION = 213;
const byte BRIGHTNESS_TARGET = 100;


byte startIndex = 0;// current array index to start lightup 

byte brightness = 0;
byte litValue = 0;

byte hueValue = 0;


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);

//  byte mA = LED_LIT_COUNT * 15;
//  FastLED.setMaxPowerInVoltsAndMilliamps(5, constrain(mA,40,200));

  startIndex = random(0,LED_NUM);

  FastLED.setBrightness(0);
  
}

void loop() {
  UpdateBrightness();
  UpdateValue();
  UpdateHue();
  UpdateDisplay();
  
  delay(DELAY_MS);
}

void UpdateBrightness() {
  if (brightness < BRIGHTNESS_TARGET) {
    brightness += BRIGHTNESS_INCREMENT;
    if (brightness > BRIGHTNESS_TARGET) {
      brightness = BRIGHTNESS_TARGET;
    }
    FastLED.setBrightness(brightness);
  }
}

void UpdateValue() {
  
  byte prevValue = litValue;

  litValue += LIT_INCREMENT;

  if (litValue < prevValue) {
    // overflowed, next LED 
    startIndex++;
    if (startIndex >= LED_NUM) {
      startIndex = 0;
    }
    
  }
  
}

void UpdateHue() {

  hueValue += HUE_BASE_INCREMENT;
  
}

void UpdateDisplay() {
  // determine lit indices 

  byte endIndex = startIndex + LED_LIT_COUNT;
  
  bool wrapped = false;
  if (endIndex > LED_NUM) {
    // loop back to zero via modulo 
    wrapped = true;
  }

  for (byte i = 0; i < LED_NUM; i++) {

    if (IsLit(i, endIndex, wrapped)) {

      bool isFalling = i == startIndex;
      bool isRising = wrapped ? 
        i == (endIndex % LED_NUM) - 1 :
        i == endIndex - 1;

      byte value = isRising ? litValue : 
        isFalling ? 255 - litValue : 255;

        leds[i] = CHSV(
          hueValue + (HUE_INDEX_INCREMENT * i),
          SATURATION,
          value);
      
    } else {

      leds[i] = CRGB::Black;
      
    }
    
  }

  FastLED.show();
  
}


bool IsLit(byte i, byte endIndex, bool wrapped) {
  if (i >= startIndex && i < endIndex) {
    return true;
  }
  if (wrapped) {
    return i < endIndex % LED_NUM;
  }
  return false;
}
