#include <time.h>
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"
#include "Logger.h"
#include <TimeLib.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>  // Add this for MQTT

// Constructor
Logger::Logger(const char* filePath) {
  _filePath = filePath;
}

// Initialize SD card and file
bool Logger::begin() {
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
    Serial.println("SD card initialization failed!");
    return false;
  }

  File logFile = SD.open(_filePath, FILE_WRITE);
  if (!logFile) {
    Serial.println("Failed to open log file for writing!");
    return false;
  }
  logFile.close();
  return true;
}

// Log with timestamp
void Logger::log(const char* key, const char* action) {
  File logFile = SD.open(_filePath, FILE_WRITE);

  if (logFile) {
    time_t currentTime = now();
    String date = formatDate(currentTime);

    StaticJsonDocument<200> doc;
    doc["timestamp"] = date;
    doc[key] = action;

    serializeJson(doc, lastLogJson);  // Save JSON for MQTT

    // Write to file and serial
    logFile.println(lastLogJson);
    Serial.println(lastLogJson);
    logFile.close();
  } else {
    Serial.println("Failed to open log file for writing!");
  }
}

// Format time
String Logger::formatDate(time_t t) {
  char dateStr[20];
  snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d %02d:%02d:%02d",
           day(t), month(t), year(t), hour(t), minute(t), second(t));
  return String(dateStr);
}

// Publish latest log via MQTT
void Logger::publish(PubSubClient& client) {
  if (lastLogJson.length() > 0) {
    client.publish("GetLogs", lastLogJson.c_str());
  }
}
