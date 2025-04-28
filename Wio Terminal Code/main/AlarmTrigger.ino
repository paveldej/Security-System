//implementation of the Alarm Trigger library

#include "AlarmTrigger.h"
#include <ChainableLED.h>
#include "Ultrasonic.h"
#include "pins.h"


ChainableLED leds(RGB_LED_DI_PIN, RGB_LED_CI_PIN, 1);
Ultrasonic ultrasonicSensor(ULTRASONIC_PIN);

//constructor
AlarmTrigger::AlarmTrigger() {
  //you may also initialize the leds here, depending on the LED library used
  leds.setColorRGB(0, 0, 50, 0);
}

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

  if (distance <= threshold) {
    return true;

  } else {
    return false;
  }
}

//the seconds denote how long should the alarm be triggered
void AlarmTrigger::triggerAlarm(int seconds) {

  int blinkRate = 500;
  unsigned long startTime = millis();

  alarmTrigger.playAlarmSound();

  //blink light red
  while (millis() - startTime <= seconds * 1000) {

    //this is needed so it reads from the arming/disarming
    //topic even while triggered
    client.loop();

    unsigned long current = millis();

    if ((current / blinkRate) % 2 == 0) {
      leds.setColorRGB(0, 50, 0, 0);  // red
    } else {
      leds.setColorRGB(0, 0, 0, 0);  // off
    }

    //check for user setting alarm to disarmed while alarm triggered
    if (armed == false) {
      turnLightGreen();
      stopAlarmSound();
      return;
    }
  }

  alarmTrigger.turnLightGreen();
  alarmTrigger.stopAlarmSound();
}
void AlarmTrigger::triggerAlarmManual(int seconds) {

  int blinkRate = 500;
  unsigned long startTime = millis();

  alarmTrigger.playAlarmSound();

  //blink light red
  while (millis() - startTime <= seconds * 1000) {

    //this is needed so it reads from the arming/disarming
    //topic even while triggered
    client.loop();

    unsigned long current = millis();

    if ((current / blinkRate) % 2 == 0) {
      leds.setColorRGB(0, 50, 0, 0);  // red
    } else {
      leds.setColorRGB(0, 0, 0, 0);  // off
    }

    //check for user setting alarm to disarmed while alarm triggered
  }

  alarmTrigger.turnLightGreen();
  alarmTrigger.stopAlarmSound();
}