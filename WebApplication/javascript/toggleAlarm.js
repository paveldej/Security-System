// const client = mqtt.connect('ws://mqtt.eclipseprojects.io:80/mqtt');

document.addEventListener("DOMContentLoaded", () => {
let s = "arm"; // initial state
const armButton = document.getElementById("armButton");
const systemStatus = document.getElementById("systemStatus");

// When connected
client.on('connect', () => {
  console.log('Connected to MQTT broker');
  client.subscribe('Status/getStatus', (err) => {
    if (!err) {
      console.log('Subscribed to topic: getStatus');
      client.publish('Status/setStatus', 'status');
    }
  });
});

//
client.on('message', function (topic, message) {
  if (topic === "Status/getStatus") {
    s = message.toString();
    console.log('Received status:', s);
    console.log(s);
    updateUI();
  }
});

// Update button and status text
function updateUI() {
  if (s === "disarm") {
    armButton.textContent = "Arm";
    systemStatus.textContent = "Status: Disarmed";
  } else if (s === "arm") {
    armButton.textContent = "Disarm";
    systemStatus.textContent = "Status: Armed";
  }
}

// Toggle the alarm status
function toggleAlarm() {
  if (s === "arm") {
    client.publish('Status/setStatus', 'disarm');
  } else {
    client.publish('Status/setStatus', 'arm');
  }
  
}
armButton.addEventListener('click', toggleAlarm);
});
