

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
  const alertBox = document.getElementById('intrusionAlert');
  alertBox.classList.remove('hidden');
  alertBox.classList.add('show');

  // Auto-hide after 10 seconds
  setTimeout(() => {
    dismissIntrusionAlert();
  }, 10000);
}

function dismissIntrusionAlert() {
  const alertBox = document.getElementById('intrusionAlert');
  alertBox.classList.remove('show');
  alertBox.classList.add('hidden');
}
