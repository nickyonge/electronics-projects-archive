#include <FastLED.h>

#define LED_NUM 15
#define LED_PIN 1
#define BTN_PIN 7

CRGB leds[LED_NUM];

const byte LED_LIT_COUNT = 10;

const byte DELAY_MS = 5;
const byte LIT_INCREMENT = 8;
const byte HUE_BASE_INCREMENT = 1;
const byte HUE_INDEX_INCREMENT = 18;
const byte BRIGHTNESS_INCREMENT = 6;

const int BTN_TIME = 10000;
int btn_value = 0;


const byte SATURATION = 213;
const byte BRIGHTNESS_TARGET = 100;


byte startIndex = 0;// current array index to start lightup 

byte brightness = 0;
byte litValue = 0;

byte hueValue = 0;

bool active = false;


void setup() {

  pinMode(BTN_PIN, INPUT);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);

//  byte mA = LED_LIT_COUNT * 15;
//  FastLED.setMaxPowerInVoltsAndMilliamps(5, constrain(mA,40,200));

  startIndex = random(0,LED_NUM);

  FastLED.setBrightness(0);
  
}

void loop() {
  UpdateInput();
  active = true;
  btn_value = 1000;
  if (active) {
    UpdateBrightness();
    UpdateValue();
    UpdateHue();
    UpdateDisplay();

    if (brightness == 0 && btn_value == 0) {
      active = false;
    }
  }
  
  delay(DELAY_MS);
}

void UpdateInput() {
  
  bool btnDown = digitalRead(BTN_PIN);
  if (btnDown) {
    btn_value = BTN_TIME;
    active = true;
  }

  if (btn_value > 0) {
    btn_value -= DELAY_MS;
    if (btn_value < 0) {
      btn_value = 0;
    }
  }
  
}

void UpdateBrightness() {

  if (!active) { return; }
  
  byte _target = GetBrightnessTarget();
  if (brightness != _target) {
    if (brightness < _target) {
      brightness += BRIGHTNESS_INCREMENT;
      if (brightness > _target) {
        brightness = _target;
      }
    } else if (brightness > _target) {
      brightness -= BRIGHTNESS_INCREMENT;
      if (brightness < _target) {
        brightness = _target;
      }
    }
    
//    FastLED.setBrightness(brightness);
FastLED.setBrightness(BRIGHTNESS_TARGET);
  }
}

byte GetBrightnessTarget() {
  if (btn_value > 0)
    return BRIGHTNESS_TARGET;
  return 0;
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
