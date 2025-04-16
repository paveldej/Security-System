import { sendMessage } from './sendmail.js';
const sendEmailMessage = sendMessage;
const alert = document.getElementById("batteryAlert");
let batterylevelemail = false;

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

    if (batteryLevel < 100) {
      batteryNotification();
      if (batterylevelemail == false){
      sendEmailMessage({
        to: 'filqwerty987@gmail.com',
        subject: 'Low battery level',
        text: 'Your battery level is below 20%!',
      });
      batterylevelemail = true;
      }
    }
    else {
      emailsended = false;
    }
  }
});

setInterval(() => {
  client.publish('wioTerminal/battery/request', '1');
}, 10000);
