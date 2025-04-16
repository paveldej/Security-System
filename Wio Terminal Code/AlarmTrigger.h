#ifndef ALARMTRIGGER_H
#define ALARMTRIGGER_H
class AlarmTrigger{
  public:
    void turnLightRed();
    void turnLightGreen();
    void playAlarmSound();
    void stopAlarmSound();
    bool objectIsClose(long threshold);
    void triggerAlarm(int seconds);
    void blinkRed(float milliseconds);
};
#endif