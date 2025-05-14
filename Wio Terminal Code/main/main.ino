#include "rpcWiFi.h"
#include <PubSubClient.h>
#include "Logger.h"
#include <TimeLib.h>
#include <ArduinoJson.h> 
#include <WiFiUdp.h>
#include <NTPClient.h>

#include <vector>
#include "AlarmTrigger.h"
#include "battery.h"
#include "display.h"
#include "buttons.h"

#define WIFI_TIMEOUT 5            //Retry after a certain amount of seconds
#define SERVER_PORT 1883          //MQTT server port 
#define MANUAL_TRIGGER_DURATION 5 //how long the manual trigger should last in seconds
#define TRIGGER_DURATION 30       //how long the non-manual trigger should last in seconds
#define TRIGGER_THRESHOLD 1     // the sum of normalized distance and sound value that is needed for alarm triggers
#define BATTERY_UPDATE_RATE 10000 //How often we send updates about the state of the battery in milliseconds
#define BATTERY_CAPACITY 650      //Preset battery capacity

const String pin = "1234";

extern std::vector<String> mainMenuOptions;
extern ScreenState screen;

extern std::vector<String> ssids;
extern int selectedSSID;
extern String passwordInput;

const char *ID = "Wio-Terminal-Client";  // Name of our device, must be unique
const char *TOPIC = "Status";  // Topic to subcribe to
const char *setStatus = "Status/setStatus";  // Topic to subcribe to
const char *getStatus = "Status/getStatus";
const char *getTrigger = "Status/getTrigger"; // This topic is meant to handle manual triggers,
const char *setTrigger = "Status/setTrigger"; // and possibly others in the future
const char *requestLogs = "requestLogs";
const char *pubBatteryLevel = "wioTerminal/battery"; // battery level publisher
// const char *server = "test.mosquitto.org"; // Server URL
const char *server = "mqtt.eclipseprojects.io"; //alternative mqtt broker
// const char *server = "broker.emqx.io"; //alternative mqtt broker

bool armed = true;
bool originalState = true;
String receivedMessage = ""; // Global string to store the message
String triggerMessage = ""; // global string to store messages for when alarm triggers
unsigned long start;
byte batteryLevel = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

AlarmTrigger alarmTrigger;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200, 60000); 
Logger logger("/log.json");


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  if (String(topic) == "Status/setStatus") {
    // Clear the previous message
    receivedMessage = "";

    // Store the message in the 'receivedMessage' string
    for (int i = 0; i < length; i++) {
      receivedMessage += (char)payload[i];  // Append each character to the string
    }

    // Print the received message
    Serial.println(receivedMessage);
    
    // Optionally, you can perform other actions based on the message
    // For example:
    if (receivedMessage == "arm") { armed = true; updateStatus();}
    else if (receivedMessage == "disarm") { armed = false; updateStatus();}
    else if(receivedMessage == "status"){
      updateStatusOnPageLoad();
    }
  } else if (String(topic)== "Status/setTrigger") {
      triggerMessage = "";
      for (int i = 0; i < length; i++) { 
        triggerMessage += (char)payload[i];
        } // Append each character to the string
      
      Serial.println(triggerMessage);
      //would make sense to have a bool here considering it should always trigger no matter the trigger message
      if (triggerMessage == "trigger") {
        client.publish(getTrigger,"trigger");
        alarmTrigger.triggerAlarmManual(MANUAL_TRIGGER_DURATION);
        client.publish(getTrigger,"notrigger");
    }
  }
  else if(String(topic) == "requestLogs"){
    receivedMessage = "";
      for (int i = 0; i < length; i++) { 
        receivedMessage += (char)payload[i];
        } // Append each character to the string
        logger.publish(client);
  }
}

void connectToWiFi() {
  start = millis();
  while (!Serial && millis() - start < WIFI_TIMEOUT) 
    ; // Wait for Serial to be ready or time out after a certain amount of seconds, 
      //this is essential, because if the device is not connected (running off the battery), it will wait forever
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssids[selectedSSID]);
  WiFi.begin(ssids[selectedSSID].c_str(), passwordInput.c_str());

  if (WiFi.isConnected()) {
    Serial.print("Connected to ");
    Serial.println(ssids[selectedSSID]);
  } else {
    Serial.print("Failed to connect to: ");
    Serial.println(ssids[selectedSSID]);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected() && WiFi.isConnected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(getStatus, "{\"message\": \"Wio Terminal is connected!\"}");
      Serial.println("Published connection message successfully!");
      // ... and resubscribe
      client.subscribe(setStatus);
      client.subscribe(setTrigger);
      Serial.print("Subcribed to: ");
      Serial.println(setStatus);
      Serial.println(setTrigger);
      client.subscribe(requestLogs);
      updateStatusOnPageLoad();
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setupTime(){
    timeClient.begin();

  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  // Set system time using TimeLib
  setTime(timeClient.getEpochTime());
}
void setupMQTT() {
  client.setServer(server, SERVER_PORT);
  client.setCallback(callback); 
}

void updateStatus()
{
  {
    if (armed == true){
      client.publish("Status/sendEmail","Armed");
      client.publish(getStatus, "arm");
       logger.log("Status","Armed");
    } else {
      client.publish("Status/sendEmail","Disarmed");
      client.publish(getStatus, "disarm");
       logger.log("Status","Disarmed");
    }
  }
}

void updateStatusOnPageLoad()
{
  {
    if (armed == true){
      client.publish(getStatus, "arm");
    } else {
      client.publish(getStatus, "disarm");
    }
  }
}

// send battery status via mqtt
void updateBattery () {
  byte soc = getBatteryLevel();
  Serial.print("sending battery info: ");
  Serial.println(soc);
  char buffer[4];
  itoa(soc, buffer, 10);
  client.publish(pubBatteryLevel, buffer);
}

void setup()
{
  Serial.begin(115200);
  setupBattery();
  initializeDisplay();
  initializeButtons();
  drawMainMenu(mainMenuOptions,0);
  armed = true;
  //creating an AlarmTrigger object
  alarmTrigger = AlarmTrigger();
  
  setupMQTT();
}

unsigned long updateBatteryPeriod = millis();
bool flag = false;


void loop()
{
  batteryLevel = getBatteryLevel();
  handleScreen(screen);


  if (!WiFi.isConnected()) {
    return;
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(armed == false){
    return;
    
  }
  
  if (flag == false){
      setupTime();  
      logger.begin();
      flag = true;
  }

  // send battery info every n/1000 seconds
  if (millis() - updateBatteryPeriod >= BATTERY_UPDATE_RATE) {
    updateBattery();
    updateBatteryPeriod = millis();
  }
  Serial.println(alarmTrigger.getNormalizedVolume());
  if(alarmTrigger.getNormalizedDistance() + alarmTrigger.getNormalizedVolume() >= TRIGGER_THRESHOLD) {
    client.publish(getTrigger, "trigger");
    Serial.println("Intruder alert published to MQTT!");
    alarmTrigger.triggerAlarm(TRIGGER_DURATION);
    logger.log("Trigger","Intruder Detected");
     Serial.println("triggered");
  } else {
   
  }
  delay(100);
}