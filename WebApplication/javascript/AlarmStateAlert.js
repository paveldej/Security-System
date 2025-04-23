const alert = document.getElementById("AlarmStateAlert");

function AlarmStateAlert() {
  alert.classList.add("show");
}


function dismissAlarmStateAlert() {
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
        AlarmStateAlert();
    } 
  }
});