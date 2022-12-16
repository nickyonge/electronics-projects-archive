#include <RotaryEncoder.h>

void setup2(){ setupENC(); }
//int _delay2 = 0;
void loop2(){
  loopENC();
//  loopEarlyENC();
//  if (_delay2 > 0) { _delay2 = max(_delay2 - LOOP_INTERVAL, 0); return; }
//  loopDelayENC();
}
//void delay2(int _delay) { _delay2 = max(_delay2,_delay); }


const int ENC_STEPS = 128;
const int ENC_COARSE_STEPS = 8;
const int ENC_INCREMENT = RGB_MAX / ENC_STEPS;

bool ENC_lastPosHeld = false;

static int ENC_pos = 0;


RotaryEncoder *_enc = nullptr;

void setupENC() {
  
  _enc = new RotaryEncoder(PIN_ENC_A, PIN_ENC_B, RotaryEncoder::LatchMode::FOUR3 );
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), checkPosition, CHANGE);
}

//void checkPosition()
IRAM_ATTR void checkPosition()
{
  _enc->tick(); // just call tick() to check the state.
  CheckState();
}

void loopENC() {
}

void CheckState() {
  // check if held - if not, coarse multiplier 
  int _dir = (int)_enc->getDirection();
  int _newPos = ENC_CurrentPos();
  if (!RGB_pressed) {
    _newPos += _dir * (ENC_COARSE_STEPS - 1);
    _dir *= ENC_COARSE_STEPS;
  }
  if (ENC_pos != _newPos) {
    UpdateState(_newPos, _dir);
  }
}

void UpdateState() {
  UpdateState(ENC_CurrentPos());
}

void UpdateState(int _newPos) {
  UpdateState(_newPos, (int)_enc->getDirection());
}
void UpdateState(int _newPos, int _dir) {
  
  int _delta = _newPos - ENC_pos;
  Serial.print("pos:");
  Serial.print(_newPos);
  Serial.print(", delta:");
  Serial.print(_delta);
  Serial.print(", dir:");
  Serial.println(_dir);
  ENC_pos = _newPos;

  if (RGB_pressed) {
    ManualTriggerHold();
  }

//  SetRGB(ENC_pos, RGB_pressed ? ENC_STEPS_COARSE_MULTI : ENC_STEPS);
//  IncrementRGB(_delta, RGB_pressed ? ENC_STEPS_COARSE_MULTI : ENC_STEPS);
//  int _increment = ENC_INCREMENT;
//  if (RGB_pressed) { _increment *= ENC_STEPS_COARSE_MULTI; }
  IncrementRGB(ENC_INCREMENT * _dir);

  _enc->setPosition(_newPos);
  
}

int ENC_CurrentPos() { 
  return mod(_enc->getPosition(), ENC_STEPS); 
}





//Encoder _enc(PIN_ENC_A, PIN_ENC_B);
//
//long ENC_prevPos = 0;
//
//
//void setupENC() {
//  ENC_prevPos = _enc.read();
//}
//
//void loopENC() {
//  long _newPos = _enc.read();
//  if (_newPos != ENC_prevPos) {
//    long _delta = _newPos - ENC_prevPos;
//    ENC_prevPos = _newPos;
//    Serial.print("ROTARY POS: ");
//    Serial.println(_newPos);
//    Serial.print("ROTARY DELTA: ");
//    Serial.println(_delta);
//    Serial.println(" ---");
//  }
//}
