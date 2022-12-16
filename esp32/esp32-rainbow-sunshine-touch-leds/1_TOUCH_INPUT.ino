
void Setup1() {
  
}
void Update1(bool delayed) {
  UpdatePinActive();
  if (!delayed) {
    Delay1(100);
    TouchTest();
  }
}
void Delay1(int ms) {
  DelayTab(1,ms);
}

const byte PIN_TOUCH_SUN = 13;
const byte PIN_TOUCH_RB1 = 12;
const byte PIN_TOUCH_RB2 = 14;
const byte PIN_TOUCH_RB3 = 27;
const byte PIN_TOUCH_RB4 = 4;
// ----------------------- do not use 0 
const byte PIN_TOUCH_RB5 = 15;
const byte PIN_TOUCH_RB6 = 2;
const byte PIN_TOUCH_RB7 = 33;// not 0


const byte TouchPin[8] = {
  PIN_TOUCH_SUN,
  PIN_TOUCH_RB1,
  PIN_TOUCH_RB2,
  PIN_TOUCH_RB3,
  PIN_TOUCH_RB4,
  PIN_TOUCH_RB5,
  PIN_TOUCH_RB6,
  PIN_TOUCH_RB7
};

const byte TouchThreshold[8] = {
  50,
  55,
  55,
  55,
  55,
  55,
  55,
  16
};

bool TouchActive[8] = {false,false,false,false,false,false,false,false};



void UpdatePinActive() {
  for (byte i = 0; i < 8; i++) {
    TouchActive[i] = GetPinActive(i);
  }
}

bool GetPinActive(byte pin) {
  bool _active = false; 
  if (GetPinTouchValue(pin) <= TouchThreshold[pin]) { 
    _active = true; 
  }
  return _active;
}

byte GetPinTouchValue(byte pin) {
  return touchRead(TouchPin[pin]);
}

void TouchTest() {
  // read current touch input for all pins 
  Serial.println("TOUCH TEST --------------------");
  for (byte i = 0; i < 8; i++) {
    Serial.print(GetName(i));
    Serial.print(":... ActiveBoolState: ");
    if (TouchActive[i]) { Serial.print("T"); } else { Serial.print("F"); }
    Serial.print(",... ActiveRaw: ");
    if (GetPinActive[i]) { Serial.print("T"); } else { Serial.print("F"); }
    Serial.print(",... Value/Threshold: "); 
    Serial.print(GetPinTouchValue(i));
    Serial.print('/');
    Serial.print(TouchThreshold[i]);
  }
  Serial.println("");
}

String GetName(byte i) {
  switch (i) {
    case 0:
    return "SUN";
    
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    String RB = "RB";
    RB.concat(i);
    return RB;
  }
  String ERR = "ERROR_UNDEFINED:";
  ERR.concat(i);
  return ERR;
}
