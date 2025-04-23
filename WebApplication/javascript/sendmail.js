import nodemailer from 'nodemailer'; // ✅ CORRECT way to import

// Create reusable transporter object using Gmail SMTP
const transporter = nodemailer.createTransport({
  service: 'gmail',
  auth: {
    user: 'pavelssystem@gmail.com',
    pass: 'ucxt uvkl rkmr gsva' // ✅ Use your Gmail app password here
  },
});

// Your email-sending function
export async function sendMessage({ to, subject, text }) {
  const mailOptions = {
    from: 'pavelssystem@gmail.com',
    to,
    subject,
    text,
  };

  try {
    const info = await transporter.sendMail(mailOptions);
    console.log('✅ Email sent:', info.response);
  } catch (error) {
    console.error('❌ Error sending email:', error);
  }
}

// Call for testing
// Uncomment below line to test directly
// sendMessage({
//   to: 'filqwerty987@gmail.com',
//   subject: 'send topic here',
//   text: 'result text here',
// });
