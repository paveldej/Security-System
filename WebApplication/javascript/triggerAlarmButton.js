
document.addEventListener("DOMContentLoaded", () => {
let state = "notrigger"; // initial state
const triggerButton = document.getElementById("triggerButton");
const triggerStatus = document.getElementById("triggerStatus");


client.on('message', function (topic, message) {
  if (topic === "Status/triggerEvents") {
    state = message.toString();
    console.log('Received trigger status:', state);
    console.log(state);
    updateUI();
  }
});
// Update button and status text
function updateUI() {
  if (state === "notrigger") {
    triggerStatus.textContent = "Status:alarm triggered";
  } 
}

// Toggle the alarm status
function publishTrigger() {
  if (state === "notrigger") {
    client.publish('Status/triggerEvents', 'trigger');
  } 
  
}
triggerButton.addEventListener('click', publishTrigger);
});
