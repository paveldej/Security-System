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
  // Attempt to initialize the SD card. Ensure SDCARD_SS_PIN and SDCARD_SPI are defined correctly.
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
    Serial.println("SD card initialization failed!");
    return false;
  }

  // Open log file in write mode, create it if it doesn't exist
  File logFile = SD.open(_filePath, FILE_APPEND);
  if (!logFile) {
    Serial.println("Failed to open log file for writing!");
    return false;
  }
  logFile.close(); // Close file after checking it
  return true;
}

// Log with timestamp
void Logger::log(const char* key, const char* action) {
  File logFile = SD.open(_filePath, FILE_APPEND);

  if (logFile) {
    time_t currentTime = now();
    String date = formatDate(currentTime);  // Format current time as a string

    StaticJsonDocument<200> doc;
    doc["timestamp"] = date;
    doc[key] = action;

    serializeJson(doc, lastLogJson);  // Serialize JSON to the string

    // Write JSON string to SD file and serial monitor
    logFile.println(lastLogJson);
    Serial.println(lastLogJson);  // Optional: prints the log to the serial monitor
    logFile.close();  // Always close the file after writing
  } else {
    Serial.println("Failed to open log file for writing!");
  }
}

// Format time into a readable string
String Logger::formatDate(time_t t) {
  char dateStr[20];
  snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d %02d:%02d:%02d",
           day(t), month(t), year(t), hour(t), minute(t), second(t));
  return String(dateStr);
}

// Publish latest log via MQTT
void Logger::publish(PubSubClient& client) {
  File logFile = SD.open(_filePath, FILE_READ);
  if (!logFile) {
    Serial.println("Failed to open log file for reading!");
    return;
  }

  String allLogs = "[";  // Start the JSON array

  bool first = true;
  while (logFile.available()) {
    String line = logFile.readStringUntil('\n');
    line.trim(); // Remove any trailing whitespace
    if (line.length() == 0) continue;  // Skip empty lines

    // Add commas between JSON objects
    if (!first) allLogs += ",";
    allLogs += line;  // Add the log entry to the JSON array
    first = false;  // Only add commas after the first entry
  }

  allLogs += "]";  // Close the JSON array
  logFile.close();  // Close the file after reading

  // Publish the logs over MQTT
  client.publish("GetLogs", allLogs.c_str());
}
