
document.addEventListener("DOMContentLoaded", () => {
let state = "notrigger"; // initial state
const triggerButton = document.getElementById("triggerButton");
const triggerStatus = document.getElementById("triggerStatus");

// When connected
client.on('connect', () => {
  client.subscribe('Status/getTrigger', (err) => {
    if (!err) {
      console.log('Subscribed to topic: getTrigger');

    }
  });
});

client.on('message', function (topic, message) {
  if (topic === "Status/getTrigger") {
    state = message.toString();
    console.log('Received trigger status:', state);
    console.log(state);
    if (state == "trigger"){
      updateUI();
    }
    
  }
});

// Update button and status text
function updateUI() {
    triggerStatus.textContent = "Status:alarm triggered";
}

// Toggle the alarm status
function publishTrigger() {
    client.publish('Status/triggerEvents', 'trigger');
}
triggerButton.addEventListener('click', publishTrigger);
});
