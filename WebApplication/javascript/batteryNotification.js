import { sendMessage } from './sendmail.js';
const sendEmailMessage = sendMessage;
const alert = document.getElementById("batteryAlert");
let isEmailSent = false


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

    if (batteryLevel < 70) {
      batteryNotification();
      if (!isEmailSent) {
        sendEmail("filqwerty987@gmail.com", "Battery Level Is Low", "The battery level is below 20%. Please check the power supplier.");
        isEmailSent = true;
      }
    } else {
      isEmailSent = false;
    }
  }
});