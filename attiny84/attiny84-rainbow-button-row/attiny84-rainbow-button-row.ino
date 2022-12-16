// ARDUINO PINS NORMAL CLOCKWISE
#define PIN2  10
#define PIN3  9
//#define PIN4  11 // skip, reset pin 
#define PIN5  8
#define PIN6  7
#define PIN7  6 // serial in
#define PIN8  5 // serial out
#define PIN9  4
#define PIN10 3
#define PIN11 2
#define PIN12 1
#define PIN13 0

// ARDUINO PINS ALTERNATE COUNTERCLOCKWISE
//#define PIN2  0
//#define PIN3  1
////#define PIN4  11 // skip, reset pin 
//#define PIN5  2
//#define PIN6  3
//#define PIN7  4 // serial in
//#define PIN8  5 // serial out
//#define PIN9  6
//#define PIN10 7
//#define PIN11 8
//#define PIN12 9
//#define PIN13 10

// DIRECT PINS
//#define PIN2  2
//#define PIN3  3
////#define PIN4  4 // skip, reset pin 
//#define PIN5  5
//#define PIN6  6
//#define PIN7  7 // serial in
//#define PIN8  8 // serial out
//#define PIN9  9
//#define PIN10 10
//#define PIN11 11
//#define PIN12 12
//#define PIN13 13

// pin definitions 
#define R_OUT PIN2
#define G_OUT PIN3
#define B_OUT PIN5
#define R_IN PIN6
#define O_IN PIN9
#define Y_IN PIN10
#define G_IN PIN11
#define B_IN PIN12
#define P_IN PIN13

void setup() {

  pinMode(R_OUT, OUTPUT);
  pinMode(G_OUT, OUTPUT);
  pinMode(B_OUT, OUTPUT);
  
  pinMode(R_IN, INPUT);
  pinMode(O_IN, INPUT);
  pinMode(Y_IN, INPUT);
  pinMode(G_IN, INPUT);
  pinMode(B_IN, INPUT);
  pinMode(P_IN, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  bool R_down = digitalRead(R_IN);
  bool O_down = digitalRead(O_IN);
  bool Y_down = digitalRead(Y_IN);
  bool G_down = digitalRead(G_IN);
  bool B_down = digitalRead(B_IN);
  bool P_down = digitalRead(P_IN);

  byte r = 0;
  byte g = 0;
  byte b = 0;

  if (R_down) {
    r = 100;
  } else if (O_down) {
    r = 50;
  }

  if (G_down) {
    g = 100;
  }

  if (B_down) {
    b = 100;
  }

  analogWrite(R_OUT, r);
  analogWrite(G_OUT, g);
  analogWrite(B_OUT, b);

}
