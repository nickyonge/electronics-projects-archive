
#define NUM_TABS 2

byte tabDelays[NUM_TABS];

void setup() {
  // begin serial
  Serial.begin(115200);

  // setup tabs
  Setup1();
  Setup2();
}

void loop() {
  delay(1);
  for (byte i = 0; i <= NUM_TABS; i++) {
    if (tabDelays[i] <= 0) {
      UpdateTab(i, false);
    } else {
      tabDelays[i]--;
      UpdateTab(i, true);
    }
  }
}

void UpdateTab(byte tab, bool delayed) {
  switch (tab) {
    case 0: break;
    case 1: Update1(delayed); break;
    case 2: Update2(delayed); break;

    default: Serial.print("ERROR: Invalid tab, cannot Update tab, tab: "); Serial.println(tab); break;  
  }
}
void DelayTab(byte tab, int ms, bool add = false) {
  if (add) {
    tabDelays[tab] += ms;
  } else {
    if (tabDelays[tab] < ms) {
      tabDelays[tab] = ms;
    }
  }
}
