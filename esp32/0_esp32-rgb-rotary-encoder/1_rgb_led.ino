void setup1(){setupRGB();}
int _delay1 = 0;
void loop1(){
  loopEarlyRGB();
  if (_delay1 > 0) { _delay1 = max(_delay1 - LOOP_INTERVAL, 0); return; }
  loopDelayRGB();
}
void delay1(int _delay) { _delay1 = max(_delay1,_delay); }

const int RGB_MAX = 1535;
const bool RGB_USES_TOGGLE = true;
const bool RGB_TOGGLE_CHANGES_CYCLE = false;
const bool RGB_CAN_CYCLE = false;

bool RGB_reverse = false;
bool RGB_cycle = true;

const int RGB_INCREMENT = 1;
const int RGB_DELAY = 3;

const int RGB_BRIGHTNESS_RGB = 255;
const int RGB_BRIGHTNESS_R = 255;
const int RGB_BRIGHTNESS_G = 160;
const int RGB_BRIGHTNESS_B = 240;

const bool RGB_HOLD_RELEASES_PRESS = false;
const int RGB_HOLDFUNCTION_TIME = 500;

static int RGB_value = 0;
static bool RGB_lastToggle = false;
static int RGB_holdTime = 0;
static bool RGB_pressed = false;


void setupRGB() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  if (RGB_USES_TOGGLE) {
    
    pinMode(TOGGLE_CYCLE_PIN, INPUT);
    RGB_lastToggle = digitalRead(TOGGLE_CYCLE_PIN);
    
  }

  RGB_value = random(0, RGB_MAX + 1);
  rgbCycle(RGB_value);

}


void loopEarlyRGB() {

  if (!RGB_USES_TOGGLE) { return; }

  // check button input 
  RGB_pressed = digitalRead(TOGGLE_CYCLE_PIN);

  if (RGB_pressed) {
    if (RGB_holdTime < RGB_HOLDFUNCTION_TIME) 
    {
      RGB_holdTime++;
      if (RGB_holdTime >= RGB_HOLDFUNCTION_TIME) {
        // first held cycle 
        ToggleHold(false);
      }
    }
  }
  
  if (RGB_pressed != RGB_lastToggle) {

//    ENC_CheckPressed();
    
    RGB_lastToggle = RGB_pressed;
//   Serial.println(RGB_pressed ? "TOGGLE ON" : "TOGGLE OFF");
    if (RGB_pressed) {
      ToggleDown();
    } 
    else 
    {
      if (RGB_holdTime >= RGB_HOLDFUNCTION_TIME) {
        ToggleUpHeld();
      }
      else 
      {
        ToggleUpShort();
      }
      RGB_holdTime = 0;
    }
  }
}

void ManualTriggerHold() {
    if (RGB_holdTime < RGB_HOLDFUNCTION_TIME) 
    {
      RGB_holdTime = RGB_HOLDFUNCTION_TIME;
      // first held cycle 
      ToggleHold(true);
    }
}

void loopDelayRGB() 
{
  if (!RGB_CAN_CYCLE) { return; }
  
  // collect held state 
  bool _held = RGB_pressed && RGB_holdTime >= RGB_HOLDFUNCTION_TIME;
  // check RGB cycle 
  if (RGB_cycle && (!RGB_pressed || (RGB_HOLD_RELEASES_PRESS && _held))) {
    delay1(RGB_DELAY);
    rgbCycle(RGB_value + RGB_INCREMENT);
  }
}

void SetRGB(int _toValue) {
  rgbCycle(_toValue);
}
void SetRGB(int _toValue, int _givenRange) {
  SetRGB(map(_toValue,0,_givenRange,0,RGB_MAX));
}

void IncrementRGB(int _byValue) {
  rgbCycle(RGB_value + _byValue);
}
void IncrementRGB(int _byValue, int _givenRange) {
  IncrementRGB(_byValue * RGB_MAX / _givenRange);
}


void ToggleDown() {  
}

void ToggleUpShort() {
  if (RGB_CAN_CYCLE && RGB_TOGGLE_CHANGES_CYCLE) {
    RGB_cycle = !RGB_cycle;
  }
}

void ToggleUpHeld() {
  
}
void ToggleHold(bool _viaManual) {
  if (!_viaManual) { ReverseRGBCycle(); }
}

void ReverseRGBCycle() {
  if (!RGB_CAN_CYCLE) { return; }
  RGB_reverse = !RGB_reverse; RGB_value = RGB_MAX - RGB_value; RGB_cycle = true;
}

void rgbCycle(int _setValue) 
{
  RGB_value = mod(_setValue, RGB_MAX);
  int _currentValue = RGB_reverse ? RGB_MAX - RGB_value : RGB_value;

  byte _step = (_currentValue / 256) % 6;
  byte _stepValue = _currentValue - (_step * 256);

  writePin(RED,   pinValue(RED,   _step, _stepValue));
  writePin(GREEN, pinValue(GREEN, _step, _stepValue));
  writePin(BLUE,  pinValue(BLUE,  _step, _stepValue));

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
