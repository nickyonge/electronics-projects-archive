#include <CapacitiveSensor.h>
#include <FastLED.h>

#define TOUCH_PIN_OUT 7 // output voltage to cap touch
#define TOUCH_PIN_IN 3  // input read capacitive touch


#define LED_NUM 8
#define LED_PIN 1



CRGB leds[LED_NUM];


const byte LED_LIT_COUNT = 6;

const byte DELAY_MS_LIT = 5;
const byte DELAY_MS_UNLIT = 25;

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


bool touch = false;
bool visible = false;


int touchedCutoff = 1000;
CapacitiveSensor   cs_6_10 = CapacitiveSensor(TOUCH_PIN_OUT,TOUCH_PIN_IN);        // 10M resistor between pins 6 & 10, pin 10 is sensor pin, add a wire and or foil if desired









void setup() {

  touch = false;
  visible = false;

  // setup touch pins 
  pinMode(TOUCH_PIN_OUT, OUTPUT);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
  
  startIndex = random(0,LED_NUM);
  FastLED.setBrightness(0);



  cs_6_10.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  
}




void loop() {

  UpdateTouch();
  
  UpdateBrightness();
  UpdateValue();
  UpdateHue();
  UpdateDisplay();
  
  delay(visible ? DELAY_MS_LIT : DELAY_MS_UNLIT);
}


void UpdateTouch() {


  long start = millis();
  long sensorValue =  cs_6_10.capacitiveSensor(30);
  touch = sensorValue > touchedCutoff;
  
}


void UpdateBrightness() {

  byte _B = touch ? BRIGHTNESS_TARGET : 0;
  
  if (brightness < _B) {
    brightness += _B;
    if (brightness > _B) {
      brightness = _B;
    }
    FastLED.setBrightness(brightness);

    visible = brightness > 0;
    
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

  if (visible) {
    hueValue += HUE_BASE_INCREMENT;
  }
  
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
