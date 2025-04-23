function sendEmail(receiver, subject, text) {
    fetch('http://localhost:3000/send-email', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        to: receiver,
        subject: subject,
        text: text,
      }),
    })
    .then(response => response.json())
    .then(data => {
      if (data.success) {
        console.log('Email sent!');
      } else {
        console.log('Failed to send email.');
      }
    })
    .catch(error => {
      console.error('Error:', error);
    });
  }