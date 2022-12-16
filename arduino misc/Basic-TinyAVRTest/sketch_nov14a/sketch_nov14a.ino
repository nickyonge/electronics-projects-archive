/*
  Fade
 
  This example shows how to fade an LED on using the analogWrite()
  function.
 
  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.
 
*/
 
int led = 0;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by
 
void setup() {
  pinMode(led, OUTPUT);
}
 
void loop() {
  analogWrite(led, brightness);
 
  brightness = brightness + fadeAmount;
 
  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0) {
     analogWrite(led, 0);   
     delay(1500);
     fadeAmount = -fadeAmount;
  }
  if(brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
 
  delay(10);
}
