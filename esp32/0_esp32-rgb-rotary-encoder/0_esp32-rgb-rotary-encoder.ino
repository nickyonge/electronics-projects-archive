#include <analogWrite.h>

const byte LOOP_INTERVAL = 1;

void setup() {
  // put your setup code here, to run once:
  setup1();
  setup2();
}

void loop() {
  // put your main code here, to run repeatedly:
  loop1();
  loop2();
  delay(LOOP_INTERVAL);
}

int mod(int x,int y){ return x<0 ? ((x+1)%y)+y-1 : x%y; }
long modLong(long x,long y){ return x<0 ? ((x+1)%y)+y-1 : x%y; }

enum : byte {

  OFF = 0,

  RED = 0,
  GREEN = 1,
  BLUE = 2,

  PIN_RED = 27,
  PIN_GREEN = 26,
  PIN_BLUE = 25,

  PIN_ENC_A = 32,
  PIN_ENC_B = 33,

  TOGGLE_CYCLE_PIN = 15,
  
};

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
