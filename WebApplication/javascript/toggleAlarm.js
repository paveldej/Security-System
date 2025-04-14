let s = "arm";
function toggleAlarm() {
  
    const armButton = document.getElementById("armButton");
    const systemStatus = document.getElementById("systemStatus");
    const client = mqtt.connect('wss://test.mosquitto.org:8081');

    const button = document.getElementById('armButton');
    client.on('message', function (topic, message) {
        if (topic == "getStatus"){
        console.log('Received message:', message.toString());
      s = message.toString();
        }
    });
    
    // When connected
    client.on('connect', () => {
      console.log('Connected to MQTT broker');
      // Subscribe to the topic
      client.subscribe('Status/setStatus', (err) => {
        if (!err) {
          console.log('Subscribed to topic: Status/setStatus');
          if(s == "arm"){
            console.log('Publishing "arm"');
            client.publish('Status/setStatus', 'arm');
            armButton.textContent = "Disarm";
            systemStatus.textContent = "Status: Armed";
            s = "disarm";
          }
          else if (s == "disarm"){
            console.log('Publishing "disarm"');
            client.publish('Status/setStatus', 'disarm');
            armButton.textContent = "Arm";
            systemStatus.textContent = "Status: Disarmed";
            s = "arm";
          }
    
        } else {
          console.error('Subscription error:', err);
        }
      });
    });
  }
