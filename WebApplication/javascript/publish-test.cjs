const mqtt = require('mqtt');

const client = mqtt.connect('mqtt://test.mosquitto.org');

client.on('connect', () => {
  client.publish('alarm/intrusion', 'INTRUDER ALERT');
  console.log('Published intruder alert!');
  client.end();
});