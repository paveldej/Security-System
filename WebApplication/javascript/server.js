const express = require('express');
const nodemailer = require('nodemailer');
const cors = require('cors');

const app = express();
app.use(cors()); // allow cross-origin requests
app.use(express.json()); // parse JSON requests

app.post('/send-email', async (req, res) => {
  const { to, subject, text } = req.body;

  const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
      user: 'pavelssystem@gmail.com',
      pass: 'ucxt uvkl rkmr gsva',
    },
  });

  try {
    await transporter.sendMail({
      from: '"Pavel Security System" <pavelssystem@gmail.com>',
      to,
      subject,
      text,
    });

    res.status(200).send({ success: true, message: 'Email sent' });
  } catch (err) {
    console.error(err);
    res.status(500).send({ success: false, message: 'Failed to send email' });
  }
});

app.listen(3000, () => console.log('Backend running on http://localhost:3000'));
