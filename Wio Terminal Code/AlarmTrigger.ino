//implementation of the Alarm Trigger library

#include "AlarmTrigger.h"
#include <ChainableLED.h>
#include "Ultrasonic.h"
#include "pins.h"


ChainableLED leds(RGB_LED_DI_PIN, RGB_LED_CI_PIN, 1);
Ultrasonic ultrasonicSensor(ULTRASONIC_PIN);

void AlarmTrigger::turnLightRed() {
  leds.setColorRGB(0, 50, 0, 0);
}

void AlarmTrigger::turnLightGreen() {
  leds.setColorRGB(0, 0, 50, 0);
}

//utilzes both the in-built and connected (port:D04) buzzers
void AlarmTrigger::playAlarmSound() {
  pinMode(WIO_BUZZER, OUTPUT);
  analogWrite(WIO_BUZZER, 64);
  digitalWrite(BUZZER_PIN, HIGH);
}

void AlarmTrigger::stopAlarmSound() {
  analogWrite(WIO_BUZZER, 0);
  digitalWrite(BUZZER_PIN, LOW);
}

bool AlarmTrigger::objectIsClose(long threshold) {

  long distance = ultrasonicSensor.MeasureInCentimeters();

  if (distance <= threshold){
    return true;

  } else {
    return false;
  }
}

//the seconds denote how long should the alarm be triggered
void AlarmTrigger::triggerAlarm(int seconds) {
  float secondCounter = 0.00;
  int blinkRate = 500; //in milliseconds
  
  alarmTrigger.playAlarmSound();

  while (secondCounter <= seconds) {
    alarmTrigger.blinkRed(blinkRate);
    secondCounter = secondCounter + (blinkRate/1000.0f);
  }

  alarmTrigger.turnLightGreen();
  alarmTrigger.stopAlarmSound();
}

//milliseconds is the blink rate
void AlarmTrigger::blinkRed(float blinkRate) {
  //off
  leds.setColorRGB(0, 0, 0, 0);
  delay(blinkRate/2);
  //red
  leds.setColorRGB(0, 50, 0, 0);
  delay(blinkRate/2);
}