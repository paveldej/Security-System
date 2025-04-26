const mqtt = require('mqtt');

const client = mqtt.connect('mqtt://test.mosquitto.org');

client.on('connect', () => {
  client.publish('Status/setTrigger', 'trigger');
  console.log('Published intruder alert!');
  client.end();
});