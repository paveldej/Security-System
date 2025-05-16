
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
  }
});

// Update button and status text
function updateUI() {
  if (s === "disarm") {
    armButton.textContent = "Arm";
    systemStatus.textContent = "Disarmed";
    
    systemStatus.classList.remove("armed");
    systemStatus.classList.add("disarmed");

    armButton.classList.remove("armed");
    armButton.classList.add("disarmed");
  } else if (s === "arm") {
    armButton.textContent = "Disarm";
    systemStatus.textContent = "Armed";
    
    systemStatus.classList.remove("disarmed");
    systemStatus.classList.add("armed");

    armButton.classList.remove("disarmed");
    armButton.classList.add("armed");
  }
}


// Toggle the alarm status
function toggleAlarm() {
  if (s === "arm") {
    client.publish('Status/setStatus', 'disarm');
    client.publish("status/getStatus", "");
  } else {
    client.publish('Status/setStatus', 'arm');
    client.publish("status/getStatus", "");
  }
}
armButton.addEventListener('click', toggleAlarm);
});
