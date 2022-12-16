#include "MapFloat.h"

const int PIN_RED = 0;
const int PIN_GREEN = 1;
const int PIN_BLUE = 4;

const byte MAX_LED_VALUE = 255;

const bool USE_BRIGHTNESS_CURVE = true;
const bool SINGLE_LED = false;

const float fullCycleTime = 3;
const int delayInterval = 10;

float rainbow = 0;
float cycle = 0;

const byte BRIGHTNESS_CURVE[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
  1,1,1,1,2,2,2,2,2,2,3,3,3,3,4,4,
  4,4,5,5,5,5,6,6,6,7,7,7,8,8,8,9,
  9,9,10,10,11,11,11,12,12,13,13,14,14,15,15,16,
  16,17,17,18,18,19,19,20,20,21,21,22,23,23,24,24,
  25,26,26,27,28,28,29,30,30,31,32,32,33,34,35,35,
  36,37,38,38,39,40,41,42,42,43,44,45,46,47,47,48,
  49,50,51,52,53,54,55,56,56,57,58,59,60,61,62,63,
  64,65,66,67,68,69,70,71,73,74,75,76,77,78,79,80,
  81,82,84,85,86,87,88,89,91,92,93,94,95,97,98,99,
  100,102,103,104,105,107,108,109,111,112,113,115,116,117,119,120,
  121,123,124,126,127,128,130,131,133,134,136,137,139,140,142,143,
  145,146,148,149,151,152,154,155,157,158,160,162,163,165,166,168,
  170,171,173,175,176,178,180,181,183,185,186,188,190,192,193,195,
  197,199,200,202,204,206,207,209,211,213,215,217,218,220,222,224,
  226,228,230,232,233,235,237,239,241,243,245,247,249,251,253,255
  };




enum LED_TYPE {
  RED   = 0,
  GREEN = 1,
  BLUE  = 2
};


// using 3 PWM pins on attiny85
// kudos: http://www.technoblogy.com/show?LE0 
volatile uint8_t* Port[] = {&OCR0A, &OCR0B, &OCR1B};
int Pin[] = {0, 1, 4};


void setup() {
  // put your setup code here, to run once:


  // 1ms * delayInterval / total duration of cycles in seconds
  cycle = (0.001 * delayInterval) / fullCycleTime;

  // setup pins
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  
  // Configure counter/timer0 for fast PWM on PB0 and PB1
  TCCR0A = 3<<COM0A0 | 3<<COM0B0 | 3<<WGM00;
  TCCR0B = 0<<WGM02 | 3<<CS00; // Optional; already set
  // Configure counter/timer1 for fast PWM on PB4
  GTCCR = 1<<PWM1B | 3<<COM1B0;
  TCCR1 = 3<<COM1A0 | 7<<CS10;

}

void loop() {

  UpdateRainbow();

  UpdateLEDs();

  delay(delayInterval);

}

void UpdateRainbow() {
  
  // update rainbow value
  rainbow += cycle;
  
  // ensure within bounds
  while (rainbow >= 1) {
    rainbow--;
  }
  
  while (rainbow < 0) {
    rainbow++;
  }
}

void UpdateLEDs() {

  SetLED(RED, GetLEDValue(RED));
  SetLED(GREEN, GetLEDValue(GREEN));
  SetLED(BLUE, GetLEDValue(BLUE));

}

void SetLED(LED_TYPE LED, int value) {
  *Port[LED] = 255 - value;// inverse intensity 
}




// 0R, 1G, 2B, analog 0-maxValue
int GetLEDValue(LED_TYPE LED) {

  // get baseline LED magnitude
  float magnitude = GetLEDMagnitude(LED);
  
  // define brightness base value 
  int brightness = constrain(
    round(magnitude * MAX_LED_VALUE),
    0, MAX_LED_VALUE);

  // using log curve or raw value? 
  if (USE_BRIGHTNESS_CURVE) {
    return BRIGHTNESS_CURVE[brightness];
  }
  
  return brightness;
}

// 0R, 1G, 2B, float 0-1
float GetLEDMagnitude(LED_TYPE LED) {

  if (SINGLE_LED) {
    if (LED == RED) {
      return rainbow >= 0.5 ? 1 - rainbow : rainbow;
    }
    return 0;
  }
  
  switch (LED) {

    case RED:
      // red
      if (rainbow < 0.3333 || rainbow > 0.6667) {
        // red active
        if (rainbow <= 0 || rainbow >= 100) {
          // 1 (special case, solid 1)
          return 1;
        } else if (rainbow < 0.3333) {
          // 1-0
          return mapFloat(rainbow, 0, 0.3333, 1, 0);
        } else {
          // 0-1
          return mapFloat(rainbow, 0.6667, 1, 0, 1);
        }
      }
      return 0;

    case GREEN:
      // green
      if (rainbow > 0 && rainbow < 0.6667) {
        // green active
        if (rainbow >= 0.3333) {
          // 1-0
          return mapFloat(rainbow, 0.3333, 0.6667, 1, 0);
        } else {
          // 0-1
          return mapFloat(rainbow, 0, 0.3333, 0, 1);
        }
      }
      return 0;

    case BLUE:
      // blue
      if (rainbow > 0.3333 || rainbow < 1) {
        // blue active
        if (rainbow >= 0.6667) {
          // 1-0
          return mapFloat(rainbow, 0.6667, 1, 1, 0);
        } else {
          // 0-1
          return mapFloat(rainbow, 0.3333, 0.6667, 0, 1);
        }
      }
      return 0;
  }

  // invalid RGB value
  return 0;
  
}
