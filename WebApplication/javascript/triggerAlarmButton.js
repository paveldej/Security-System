
document.addEventListener("DOMContentLoaded", () => {
let state = "notrigger"; // initial state
const triggerButton = document.getElementById("triggerAlarmButton");

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
  }
});

// Trigger the alarm
function publishTrigger() {
    client.publish('Status/setTrigger', 'trigger');
}
triggerButton.addEventListener('click', publishTrigger);
});
