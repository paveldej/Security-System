//implementation of the Alarm Trigger library

#include "AlarmTrigger.h"
#include <ChainableLED.h>
#include "Ultrasonic.h"

//hard-coded ports for now, might change it later
ChainableLED leds(2, 3, 1);
Ultrasonic ultrasonicSensor(D0);

void AlarmTrigger::turnLightRed()
{
  leds.setColorRGB(0, 50, 0, 0);
}

void AlarmTrigger::turnLightGreen()
{
  leds.setColorRGB(0, 0, 50, 0);
}

//utilzes both the in-built and connected (port:D04) buzzers
void AlarmTrigger::playAlarmSound()
{
  pinMode(WIO_BUZZER, OUTPUT);
  analogWrite(WIO_BUZZER, 64);
  digitalWrite(4, HIGH);
}

void AlarmTrigger::stopAlarmSound(){
  analogWrite(WIO_BUZZER, 0);
  digitalWrite(4, LOW);
}

bool AlarmTrigger::objectIsClose(long threshold)
{
  long distance = ultrasonicSensor.MeasureInCentimeters();
  if (distance <= threshold){
    return true;
  } else {
    return false;
  }
}

//the seconds denote how long should the alarm be triggered
void AlarmTrigger::triggerAlarm(int seconds){

  alarmTrigger.playAlarmSound();
  alarmTrigger.turnLightRed();
  
  delay(seconds*1000);
  
  alarmTrigger.turnLightGreen();
  alarmTrigger.stopAlarmSound();
}