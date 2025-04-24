#include "rpcWiFi.h"
#include <PubSubClient.h>
#include "AlarmTrigger.h"
#include <SparkFunBQ27441.h>

// Update these with values suitable for your network.
const char *ssid = "forza juve";      // your network SSID
const char *password = "filqwerty"; // your network password

const char *ID = "Wio-Terminal-Client";  // Name of our device, must be unique
const char *TOPIC = "Status";  // Topic to subcribe to
const char *subTopic1 = "Status/setStatus";  // Topic to subcribe to
const char *subTopic2 = "Status/getStatus";
const char *pubBatteryLevel = "wioTerminal/battery"; // battery level publisher
const char *server = "test.mosquitto.org"; // Server URL
// const char *server = "mqtt.eclipseprojects.io"; //alternative mqtt broker
// const char *server = "broker.emqx.io"; //alternative mqtt broker

bool armed = true;
String receivedMessage = ""; // Global string to store the message
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
      Serial.print("Subcribed to: ");
      Serial.println(subTopic1);
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

void setup()
{
  armed = true;
  Serial.begin(115200);
  setupBattery();
  start = millis();
  while (!Serial && millis() - start < 5000) 
    ; // Wait for Serial to be ready or time out after 5 seconds, 
      //this is essentail, because if the device is not connected (running off the battery), it will wait forever
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    WiFi.begin(ssid, password);
    // wait 1 second for re-trying
    delay(1000);
  }
  
  Serial.print("Connected to ");
  Serial.println(ssid);
  delay(500);

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

unsigned long updateBatteryPeriod = millis();
unsigned long objectDetectedStart = millis();

void loop()
{
  if (!client.connected()) {
    reconnect();
  }  
  client.loop();
  
  // send battery info every n/1000 seconds
  if (millis() - updateBatteryPeriod >= 10000) {
    updateBattery();
    updateBatteryPeriod = millis();
  }

  if (armed == false){
    return;
  }

  //we trigger it when its less than or equal to 150 cms and it triggers for 30 seconds
  
  if (alarmTrigger.objectIsClose(150)){
    if(millis() - objectDetectedStart >= 15000) {
      alarmTrigger.triggerAlarm(30);
    }
  } else {
    objectDetectedStart = millis();
  }
  delay(500);
}