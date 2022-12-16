const int pin0 = 0;
const int pin1 = 1;
const int pin2 = 2;

const bool change = true;

const int staticSpeed = 1000;

const int intervalSpeedLow = 2;
const int intervalSpeedHigh = 150;

const int intervalMax = 250;
const int intervalMin = 10;


int interval = change ? intervalMin : staticSpeed;
bool rising = false;



void setup()
{
  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void loop()
{
  digitalWrite(pin0, HIGH);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  
  UpdateInterval();
  
  digitalWrite(pin0, LOW);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  
  UpdateInterval();
  
  digitalWrite(pin0, LOW);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);

  UpdateInterval();
}

void UpdateInterval() 
{
  // check if change should occur 
  if (!change) { return; }

  // check rising or falling

  //0 to 1 based on progress
  float value = (interval - intervalMin) / (intervalMax - intervalMin);
  
  float intervalSpeed = ((intervalSpeedHigh - intervalSpeedLow) * value) + intervalSpeedLow;

  interval += rising ? intervalSpeed : -intervalSpeed;
  
  if ((rising && interval >= intervalMax) ||
     (!rising && interval <= intervalMin)) {
      rising = !rising;
  }
  
  delay(interval);
}
