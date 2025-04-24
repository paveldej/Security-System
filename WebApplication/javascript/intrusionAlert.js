const intrusionClient = mqtt.connect('wss://test.mosquitto.org:8081');

intrusionClient.on('connect', () => {
  console.log('Frontend connected to MQTT for intruder alert!');
  intrusionClient.subscribe('alarm/intrusion');
});

intrusionClient.on('message', (topic, message) => {
  if (topic === 'alarm/intrusion' && message.toString() === 'INTRUDER ALERT') {
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
