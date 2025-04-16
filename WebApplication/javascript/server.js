// server.js
import express from 'express';
import nodemailer from 'nodemailer';
import cors from 'cors';
import dotenv from 'dotenv';

dotenv.config();

const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

const transporter = nodemailer.createTransport({
  service: 'gmail',
  auth: {
    user: 'pavelssystem@gmail.com',
    pass: 'ucxt uvkl rkmr gsva', // Use your Gmail App Password
  },
});

app.post('/send-email', async (req, res) => {
  const { to, subject, text } = req.body;

  const mailOptions = {
    from: 'pavelssystem@gmail.com',
    to,
    subject,
    text,
  };

  try {
    const info = await transporter.sendMail(mailOptions);
    console.log('✅ Email sent:', info.response);
    res.status(200).json({ success: true, message: 'Email sent' });
  } catch (error) {
    console.error('❌ Error sending email:', error);
    res.status(500).json({ success: false, message: 'Failed to send email', error });
  }
});

app.listen(PORT, () => {
  console.log(`✅ Server running on http://localhost:${PORT}`);
});
