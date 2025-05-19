
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
      client.publish('requestLogs', 'getLogs');
      client.publish('Status/setStatus', 'status');
    }
  });
});

//
client.on('message', function (topic, message) {
  if (topic === "Status/getStatus") {
    s = message.toString();
    updateUI()
    console.log('Received status:', s);
    console.log(s);
    isStatusReceived = true;
  }
});

// Update button and status text
function updateUI() {
  if (s === "disarm") {
    armButton.textContent = "Arm";
    systemStatus.textContent = "Disarmed";
  } else if (s === "arm") {
    armButton.textContent = "Disarm";
    systemStatus.textContent = "Armed";
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
