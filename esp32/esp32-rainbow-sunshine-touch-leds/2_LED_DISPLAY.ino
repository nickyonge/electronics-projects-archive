#include <FastLED.h>

#define LED_NUM_RBW 12
#define LED_NUM_SUN 3

#define LED_DATAPIN_RBW 22
#define LED_DATAPIN_SUN 19 

CRGB LEDs_Rainbow[LED_NUM_RBW];
CRGB LEDs_Sun[LED_NUM_SUN];

byte rgbHue = 0;
byte sunValue = 0;

bool sunRising = true;
bool sunIsMoon = false;

bool sunToggle = false;// simple flag so sun is modified at 1/2 rainbow speed 

void Setup2() {
  sunRising = true;
  sunValue = 0;
  FastLED.addLeds<WS2812, LED_DATAPIN_RBW, GRB>(LEDs_Rainbow, LED_NUM_RBW);
  FastLED.addLeds<WS2812, LED_DATAPIN_SUN, GRB>(LEDs_Sun, LED_NUM_SUN);
}

void Update2(bool delayed) {

  if (delayed) { return; }

  for (int i = 0; i < LED_NUM_RBW; i++) {
    LEDs_Rainbow[i] = CHSV(rgbHue,255,100);
  }
  for (int i = 0; i < LED_NUM_SUN; i++) {
    LEDs_Sun[i] = CHSV(sunIsMoon ? 150 : 48,sunIsMoon ? 64 : 213,sunValue);
  }
  
  FastLED.show();

  if (rgbHue >= 255) {
    rgbHue = 0;
  } else {
    rgbHue++;
  }

  sunToggle = !sunToggle;
  if (sunToggle) {
    if (sunRising) {
      sunValue++;
      if (sunValue >= 255) {
        sunRising = false;
      }
    } else {
      sunValue--;
      if (sunValue <= 0) {
        sunRising = true;
        sunIsMoon = !sunIsMoon;
      }
    }
  }
  
  Delay2(3);
}
void Delay2(int ms) {
  DelayTab(2,ms);
}
