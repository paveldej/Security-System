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

// // Publish latest log via MQTT
void Logger::publish(PubSubClient& client) {
  if (!client.connected()) {
    Serial.println("MQTT client not connected.");
    return;
  }

  File logFile = SD.open(_filePath, FILE_READ);
  if (!logFile) {
    Serial.println("Failed to open log file for reading!");
    return;
  }

  long fileSize = logFile.size();
  long pos = fileSize - 1;
  int lineCount = 0;
  String currentLine = "";
  String lastLines[15];
  int index = 14;

  while (pos >= 0 && lineCount < 15) {
    logFile.seek(pos);
    char c = logFile.read();
    if (c == '\n') {
      if (currentLine.length() > 0) {
        lastLines[index] = currentLine;
        index--;
        lineCount++;
        currentLine = "";
      }
    } else {
      currentLine = c + currentLine;
    }
    pos--;
  }

  if (currentLine.length() > 0 && lineCount < 15) {
    lastLines[index] = currentLine;
    index--;
    lineCount++;
  }

  for (int i = index + 1; i < 15; i++) {
    String line = lastLines[i];
    line.trim();

    if (line.length() > 0) {
      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, line);
      if (error) {
        Serial.println("Invalid JSON, skipping:");
        Serial.println(line);
        continue;
      }

      bool success = client.publish("getLogs", line.c_str());
      if (!success) {
        Serial.println("Failed to publish log line.");
      }
      delay(10);
    }
  }

  logFile.close();
  Serial.println("Last 15 logs published.");
}

