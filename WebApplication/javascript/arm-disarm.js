const mqtt = require('mqtt');
// Connect to the public Mosquitto broker
const client = mqtt.connect('mqtt://test.mosquitto.org');
const button = document.getElementById('armButton');
let s = "arm";
client.on('message', function (topic, message) {
    console.log('Received message:', message.toString());
    s = message.toString();
});

// When connected
client.on('connect', () => {
  console.log('Connected to MQTT broker');
  // Subscribe to the topic
  client.subscribe('Status/setStatus', (err) => {
    if (!err) {
      console.log('Subscribed to topic: Status/setStatus');
      if(s = "arm"){
        console.log('Publishing "arm"');
        client.publish('Status/setStatus', 'arm');
        s = "disarm";
      }
      else if ("disarm"){
        console.log('Publishing "disarm"');
        client.publish('Status/setStatus', 'disarm');
        s = "arm";
      }

    } else {
      console.error('Subscription error:', err);
    }
  });
});

// Handle incoming messages
client.on('message', (topic, message) => {
  console.log(`📨 Message received on [${topic}]: ${message.toString()}`);
});
