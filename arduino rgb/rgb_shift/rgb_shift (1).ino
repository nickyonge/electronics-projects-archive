#include <analogWrite.h>

void setup(){setupRGB();}
void loop(){loopRGB(); delay(1);}

enum : byte {

  OFF = 0,

  RED = 0,
  GREEN = 1,
  BLUE = 2,

  PIN_RED = 25,
  PIN_GREEN = 26,
  PIN_BLUE = 27,

  TOGGLE_CYCLE_PIN = 15,
  
};

const int RGB_MAX = 1535;
const bool RGB_USER_TOGGLE_CYCLE_PIN = true;

bool RGB_reverse = false;
bool RGB_cycle = true;

const int RGB_INCREMENT = 1;
const int RGB_DELAY = 3;

const int RGB_BRIGHTNESS_RGB = 255;
const int RGB_BRIGHTNESS_R = 235;
const int RGB_BRIGHTNESS_G = 180;
const int RGB_BRIGHTNESS_B = 255;

const int RGB_HOLDFUNCTION_TIME = 500;

int RGB_value = 0;
bool RGB_lastToggle = false;
int RGB_holdTime = 0;
bool RGB_triggeredHold = false;


void setupRGB() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  if (RGB_USER_TOGGLE_CYCLE_PIN) {
    
    pinMode(TOGGLE_CYCLE_PIN, INPUT);
    RGB_lastToggle = digitalRead(TOGGLE_CYCLE_PIN);
    
  }

  RGB_value = random(0, RGB_MAX + 1);
  rgbCycle(RGB_value);

}


int RGB_internalDelay = 0;
void loopRGB() {

  // check button input 
  bool _toggle = digitalRead(TOGGLE_CYCLE_PIN);

  if (_toggle) {
    if (RGB_holdTime < RGB_HOLDFUNCTION_TIME) {
      RGB_holdTime++;
    }
  }
  
  if (_toggle != RGB_lastToggle) {
    RGB_lastToggle = _toggle;
//   Serial.println(_toggle ? "TOGGLE ON" : "TOGGLE OFF");
    if (_toggle) {
      ToggleDown();
    } 
    else 
    {
      if (RGB_holdTime < RGB_HOLDFUNCTION_TIME) {
        ToggleUp();
      }
      else 
      {
        ToggleHold();
      }
      RGB_holdTime = 0;
    }
  }

  // check internal delay 
  if (RGB_internalDelay > 0) { 
    RGB_internalDelay--; 
    return; 
  }

  // check RGB cycle 
  if (RGB_cycle && !_toggle) {
    RGB_internalDelay += RGB_DELAY;
    RGB_value += RGB_INCREMENT;
    rgbCycle(RGB_value);
  }
}

void ToggleDown() {  
}
void ToggleUp() {
  RGB_cycle = !RGB_cycle;
}
void ToggleHold() {
  RGB_reverse = !RGB_reverse; RGB_value = RGB_MAX - RGB_value; RGB_cycle = true;
}

void rgbCycle(int _input) 
{
  
  RGB_value = abs(_input) % RGB_MAX;
  int _currentValue = RGB_reverse ? RGB_MAX - RGB_value : RGB_value;

  byte _step = (_currentValue / 256) % 6;
  byte _stepValue = _currentValue - (_step * 256);

  writePin(RED,   pinValue(RED,   _step, _stepValue));
  writePin(GREEN, pinValue(GREEN, _step, _stepValue));
  writePin(BLUE,  pinValue(BLUE,  _step, _stepValue));

}

void writePin(byte _color, byte _writeValue) {
  analogWrite(getPinFromColor(_color), _writeValue);
}

byte getPinFromColor(byte _color) {
  switch (_color) {
    case RED: return PIN_RED;
    case GREEN: return PIN_GREEN;
    case BLUE: return PIN_BLUE;
  }
  Serial.print("Invalid pin color, cannot determine pin to write, returning 0, Color:");
  Serial.println(_color);
  return 0;
}

byte GET_ON(byte _color) {
  byte _onValue = 255;
  switch (_color) {
    case RED: _onValue = RGB_BRIGHTNESS_R; break;
    case GREEN: _onValue = RGB_BRIGHTNESS_G; break;
    case BLUE: _onValue = RGB_BRIGHTNESS_B; break;
  }
  return map(_onValue, 0, 255, 0, RGB_BRIGHTNESS_RGB); 
}
  
byte GET_CHANGE(byte _color, byte _step, byte _stepValue)
{
  byte _onValue = GET_ON(_color);
  _stepValue = map(_stepValue, 0, 255, 0, _onValue);
  return _step % 2 == 0 ?
    _stepValue : // rising
    _onValue - _stepValue; // falling
}

byte pinValue(byte _color, byte _step, byte _stepValue) 
{

  switch (_step) {
    
    case 0:
    case 6:
    switch (_color) {
      // R1 Gr B0
      case RED:
      return GET_ON(_color);
      case GREEN:
      return GET_CHANGE(_color, _step, _stepValue);
      case BLUE:
      return OFF;
    }
    break;

    case 1:
    // Rf G1 B0
    switch (_color) {
      case RED:
      return GET_CHANGE(_color, _step, _stepValue);
      case GREEN:
      return GET_ON(_color);
      case BLUE:
      return OFF;
    }
    break;

    case 2:
    // R0 G1 Br
    switch (_color) {
      case RED:
      return OFF;
      case GREEN:
      return GET_ON(_color);
      case BLUE:
      return GET_CHANGE(_color, _step, _stepValue);
    }
    break;

    case 3:
    // R0 Gf B1
    switch (_color) {
      case RED:
      return OFF;
      case GREEN:
      return GET_CHANGE(_color, _step, _stepValue);
      case BLUE:
      return GET_ON(_color);
    }
    break;

    case 4:
    // Rr G0 B1
    switch (_color) {
      case RED:
      return GET_CHANGE(_color, _step, _stepValue);
      case GREEN:
      return OFF;
      case BLUE:
      return GET_ON(_color);
    }
    break;

    case 5:
    // R1 G0 Bf
    switch (_color) {
      case RED:
      return GET_ON(_color);
      case GREEN:
      return OFF;
      case BLUE:
      return GET_CHANGE(_color, _step, _stepValue);
    }
    break;
    
  }

  Serial.print("Failed parsing rainbow, Color:");
  Serial.print(_color);
  Serial.print(", Step:");
  Serial.print(_step);
  Serial.print(", StepValue:");
  Serial.print(_stepValue);
  Serial.println(", cannot get pin color, returning 0");
  return 0;
}
