require('dotenv').config();
const express = require('express');
const nodemailer = require('nodemailer');
const cors = require('cors');
const mqtt = require('mqtt');

const app = express();
const client = mqtt.connect('wss://test.mosquitto.org:8081');

app.use(cors());
app.use(express.json());

let isEmailSent = false;

app.post('/send-email', async (req, res) => {
  const { to, subject, text } = req.body;

  try {
    await sendEmail(to, subject, text);
    res.status(200).send({ success: true, message: 'Email sent' });
  } catch (err) {
    console.error(err);
    res.status(500).send({ success: false, message: 'Failed to send email' });
  }
});

async function sendEmail(to, subject, text) {
  const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
      user: process.env.EMAIL_USER,
      pass: process.env.EMAIL_PASS,
    },
  });

  await transporter.sendMail({
    from: '"Pavel Security System" <' + process.env.EMAIL_USER + '>',
    to,
    subject,
    text,
  });
}

client.on('connect', () => {
  console.log('Connected to MQTT broker');
  client.subscribe('Status/getStatus');
  client.subscribe('wioTerminal/battery');
  client.subscribe("Status/sendEmail");
  client.subscribe("Status/getTrigger");
  client.publish('Status/setStatus', 'status');
  if (process.argv[2] === 'send') {
    console.log("Deployed");
    process.exit(0);
  }
});

// FIXED: combined into one 'message' event listener
client.on('message', async (topic, message) => {
  try {
    const msg = message.toString();

    switch (topic) {
      case 'wioTerminal/battery':
        const batteryLevel = parseInt(msg);
        console.log("Battery Level:", batteryLevel);

        if (batteryLevel < 20) {
          if (!isEmailSent) {
            await sendEmail(
              "filqwerty987@gmail.com",
              "Battery Level Is Low",
              "The battery level is below 20%. Please check the power supply."
            );
            isEmailSent = true;
          }
        } else {
          isEmailSent = false;
        }
        break;

      case 'Status/sendEmail':
        console.log(msg);
        if (msg === "Armed" || msg === "Disarmed") {
          await sendEmail(
            //make it a variable plsssssss
            "filqwerty987@gmail.com",
            "The system status is " + msg,
            "The system status has been changed to: " + msg
          );
        }
        break;

      //we now don't distinguish between manual and 
      //non-manual triggering in the mail
      case 'Status/getTrigger':
        if (msg === 'trigger') {
          console.log("Intrusion Detected!");
          await sendEmail(
            "filqwerty987@gmail.com",
            "Intrusion Alert Detected",
            "A possible intrusion was detected by the alarm system. Please check your premises immediately."
          );
        }
        break;
    }
  } catch (err) {
    console.error('Error handling MQTT message:', err);
  }
});

client.on('error', (err) => {
  console.error('MQTT Error:', err);
});

process.on('SIGINT', () => {
  console.log("Gracefully shutting down...");
  client.end();
  process.exit();
});

app.listen(3000, () => console.log('Backend running on http://localhost:3000'));
