

client.on('connect', () => {
  console.log('Frontend connected to MQTT for intruder alert!');
  client.subscribe('Status/getTrigger');
});

client.on('message', (topic, message) => {
  if (topic === 'Status/getTrigger' && message.toString() === 'trigger') {
    console.log("sent the notification for intruder");
    showIntrusionAlert();
  }
});

function showIntrusionAlert() {
  const alert = document.getElementById("intrusionAlert");
  alert.style.display = "flex"; // make it visible
  requestAnimationFrame(() => {
    alert.classList.remove("hidden");
  });
  // Auto-hide after 5 seconds
  setTimeout(() => {
    dismissIntrusionAlert();
  }, 5000);
}

function dismissIntrusionAlert() {
  const alert = document.getElementById("intrusionAlert");
  alert.classList.add("hidden");

  setTimeout(() => {
    alert.style.display = "none";
  }, 600); // match CSS transition
}
