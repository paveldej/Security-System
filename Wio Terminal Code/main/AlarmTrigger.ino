//implementation of the Alarm Trigger library

#include "AlarmTrigger.h"
#include <ChainableLED.h>
#include "Ultrasonic.h"
#include "pins.h"

#define LED_BLINK_RATE 500 // delay between blinks in milliseconds


#define MAX_DISTANCE_CM 200.0 
#define MIN_DISTANCE_CM 0.0

#define MIC_IDLE 512  //value for "silence"
#define MIC_MAX_DEVIATION 512 //max deviation from 512 towards 0 and 1023

#define SOUND_NORMALIZED_WEIGHT 1.5
#define DISTANCE_NORMALIZED_WEIGHT 1


ChainableLED leds(RGB_LED_DI_PIN, RGB_LED_CI_PIN, 1);
Ultrasonic ultrasonicSensor(ULTRASONIC_PIN);
unsigned long distance;

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

  distance = ultrasonicSensor.MeasureInCentimeters();

  if (distance <= threshold) {
    return true;

  } else {
    return false;
  }
}

//the seconds denote how long should the alarm be triggered
void AlarmTrigger::triggerAlarm(int seconds) {

  unsigned long startTime = millis();

  alarmTrigger.playAlarmSound();

  //blink light red
  while (millis() - startTime <= seconds * 1000) {

    //this is needed so it reads from the arming/disarming
    //topic even while triggered
    client.loop();

    unsigned long current = millis();

    if ((current / LED_BLINK_RATE) % 2 == 0) {
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

  unsigned long startTime = millis();

  alarmTrigger.playAlarmSound();

  //blink light red
  while (millis() - startTime <= seconds * 1000) {

    //this is needed so it reads from the arming/disarming
    //topic even while triggered
    client.loop();

    unsigned long current = millis();

    if ((current / LED_BLINK_RATE) % 2 == 0) {
      leds.setColorRGB(0, 50, 0, 0);  // red
    } else {
      leds.setColorRGB(0, 0, 0, 0);  // off
    }
  }

  alarmTrigger.turnLightGreen();
  alarmTrigger.stopAlarmSound();
}
float AlarmTrigger::getNormalizedDistance() {
  distance = ultrasonicSensor.MeasureInCentimeters();

  // Clamp between min and max distance
  distance = constrain(distance, MIN_DISTANCE_CM, MAX_DISTANCE_CM);

  // normalize so closer to 0 cm away = closer to 1.0
  float norm = 1.0 - (DISTANCE_NORMALIZED_WEIGHT *(distance - MIN_DISTANCE_CM) / (MAX_DISTANCE_CM - MIN_DISTANCE_CM));
  return norm;
}

float AlarmTrigger::getNormalizedVolume() {
  int raw = analogRead(LOUDNESS_SENSOR_PIN);

  int deviation = raw/2;   // Deviation from silence 
  Serial.println(deviation);
  float norm = SOUND_NORMALIZED_WEIGHT* ((float)deviation / MIC_MAX_DEVIATION);
  norm =  constrain(norm, 0.0, 1.0);     
  return norm;
}