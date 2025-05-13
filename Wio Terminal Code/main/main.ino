#include "rpcWiFi.h"
#include <PubSubClient.h>
#include <SparkFunBQ27441.h>
#include <vector>
#include "AlarmTrigger.h"
#include "display.h"
#include "buttons.h"


extern std::vector<String> mainMenuOptions;
extern ScreenState screen;

extern std::vector<String> ssids;
extern int selectedSSID;
extern String passwordInput;

const char *ID = "Wio-Terminal-Client";  // Name of our device, must be unique
const char *TOPIC = "Status";  // Topic to subcribe to
const char *subTopic1 = "Status/setStatus";  // Topic to subcribe to
const char *subTopic2 = "Status/getStatus";
const char *getTrigger = "Status/getTrigger"; // This topic is meant to handle manual triggers,
const char *setTrigger = "Status/setTrigger"; // and possibly others in the future
const char *pubBatteryLevel = "wioTerminal/battery"; // battery level publisher
const char *server = "test.mosquitto.org"; // Server URL
// const char *server = "mqtt.eclipseprojects.io"; //alternative mqtt broker
// const char *server = "broker.emqx.io"; //alternative mqtt broker

bool armed = true;
bool originalState = true;
String receivedMessage = ""; // Global string to store the message
String triggerMessage = ""; // global string to store messages for when alarm triggers
unsigned long start;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

AlarmTrigger alarmTrigger;

const unsigned int BATTERY_CAPACITY = 650; // Set Wio Terminal Battery's Capacity 

void setupBattery(void)
{
  // Use lipo.begin() to initialize the BQ27441-G1A and confirm that it's
  // connected and communicating.
  if (!lipo.begin()) // begin() will return true if communication is successful
  {
  // If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    Serial.println("  Check wiring and try again.");
    Serial.println("  (Battery must be plugged into Battery Babysitter!)");
    while (1) ;
  }
  Serial.println("Connected to BQ27441!");
  
  // Uset lipo.setCapacity(BATTERY_CAPACITY) to set the design capacity
  // of your battery.
  lipo.setCapacity(BATTERY_CAPACITY);
}

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
        client.publish("Status/getTrigger","trigger");
        alarmTrigger.triggerAlarmManual(5);
        client.publish("Status/getTrigger","notrigger");
    }
  }
}

void connectToWiFi() {
  start = millis();
  while (!Serial && millis() - start < 5000) 
    ; // Wait for Serial to be ready or time out after 5 seconds, 
      //this is essentail, because if the device is not connected (running off the battery), it will wait forever
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
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(subTopic2, "{\"message\": \"Wio Terminal is connected!\"}");
      Serial.println("Published connection message successfully!");
      // ... and resubscribe
      client.subscribe(subTopic1);
      client.subscribe(setTrigger);
      Serial.print("Subcribed to: ");
      Serial.println(subTopic1);
      Serial.println(setTrigger);
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

void setupMQTT() {
  client.setServer(server, 1883);
  client.setCallback(callback);
}

//send battery status via mqtt
void updateStatus()
{
  {
    if (armed == true){
      client.publish("Status/sendEmail","Armed");
      client.publish(subTopic2, "arm");
    } else {
      client.publish("Status/sendEmail","Disarmed");
      client.publish(subTopic2, "disarm");
    }
  }
}

void updateStatusOnPageLoad()
{
  {
    if (armed == true){
      client.publish(subTopic2, "arm");
    } else {
      client.publish(subTopic2, "disarm");
    }
  }
}

// send battery status via mqtt
void updateBattery () {
  byte soc = lipo.soc(); // read battery percentage
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
unsigned long objectDetectedStart = millis();

void loop()
{
  handleScreen(screen);

  if (!WiFi.isConnected()) {
    objectDetectedStart = millis();
    return;
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //calling displaying alarm state functionality from TerminalDisplay
  TerminalDisplay::displayStatus(armed);
  
  // send battery info every n/1000 seconds
  if (millis() - updateBatteryPeriod >= 10000) {
    updateBattery();
    updateBatteryPeriod = millis();
  }

  if (armed == false){
    objectDetectedStart = millis();
    return;
  }

  //we trigger it when its less than or equal to 150 cms and it triggers for 30 seconds
  if (alarmTrigger.objectIsClose(40)){
    if(millis() - objectDetectedStart >= 15000) {
      client.publish(getTrigger, "trigger");
      Serial.println("Intruder alert published to MQTT!");
      alarmTrigger.triggerAlarm(30);
    }
  } else {
    objectDetectedStart = millis();
  }
  delay(100);
}