#include "rpcWiFi.h"
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char *ssid = "Galaxy Note20 Ultra 5Gb10f";      // your network SSID
const char *password = "gzjx4245"; // your network password

const char *ID = "Wio-Terminal-Client";  // Name of our device, must be unique
const char *TOPIC = "Status";  // Topic to subcribe to
const char *subTopic1 = "Status/setStatus";  // Topic to subcribe to
const char *subTopic2 = "Status/getStatus";
const char *server = "test.mosquitto.org"; // Server URL
bool previousStatus = true;
bool armed;
String receivedMessage = ""; // Global string to store the message
unsigned long start;



WiFiClient wifiClient;
PubSubClient client(wifiClient);



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
   if (receivedMessage == "arm") { armed = true; }
   if (receivedMessage == "disarm") { armed = false; }
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

void updateStatus()
{
  if (previousStatus != armed)
  {
    if (armed == true){
      client.publish(subTopic2, "armed");
    } else {
      client.publish(subTopic2, "disarmed");
    }
    previousStatus = armed;
  }
}


void loop()
{
  if (!client.connected()) {
    reconnect();
  }  
  client.loop();
  updateStatus();
  if (armed == false){
    return;
  }
}