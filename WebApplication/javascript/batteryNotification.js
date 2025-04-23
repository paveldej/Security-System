const alert = document.getElementById("batteryAlert");



function batteryNotification() {
  alert.classList.add("show");
}


function dismissBatteryAlert() {
  alert.classList.remove("show");
}


client.on('connect', () => {
  console.log('Connected to MQTT broker');

  client.subscribe('wioTerminal/battery', (err) => {
    if (!err) {
      console.log('Subscribed to wioTerminal/battery');
    }
  });
});


client.on('message', function (topic, message) {
  if (topic === 'wioTerminal/battery') {
    const batteryLevel = parseInt(message.toString());
    console.log("Battery Level:", batteryLevel);

    if (batteryLevel < 20) {
      batteryNotification();
     
    } 
  }
});