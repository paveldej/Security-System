const alert = document.getElementById("batteryAlert");
const batteryPercentageElement = document.getElementById("batteryPercentage");



function batteryNotification() {
  console.log("Battery level is low. Displaying notification...");
  alert.classList.add("show");  // Add the "show" class to make the alert visible
}


function dismissBatteryAlert() {
  alert.classList.remove("show");  // Remove the "show" class to hide the alert
}


client.on('connect', () => {
  console.log('Connected to MQTT broker');

  client.subscribe('wioTerminal/battery', (err) => {
    if (!err) {
      console.log('Subscribed to wioTerminal/battery');
    } else {
      console.error('Error subscribing to wioTerminal/battery:', err);
    }
  });
});


client.on('message', function (topic, message) {
  console.log(`Received message on topic ${topic}: ${message.toString()}`);
  
  if (topic === 'wioTerminal/battery') {
    const batteryLevel = parseInt(message.toString(), 10); // Parse the message to an integer
    console.log("Battery Level:", batteryLevel);
    
    batteryPercentageElement.innerHTML = `Battery: ${batteryLevel}%`;

    if (batteryLevel < 20) {
      batteryNotification();
      
    }
  }
});