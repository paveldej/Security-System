const { createProxyMiddleware } = require('http-proxy-middleware');
const express = require('express');
const bodyParser = require('body-parser');
const path = require('path');
const dotenv = require('dotenv');

dotenv.config();

const app = express();
const PORT = 3001;

let isLoggedIn = false; // sessionless login tracking for simple flow (I tried to do it with session and cookies but it was really complicated, but might do it later)

app.use(bodyParser.json());

// Serve login.html freely
app.get('/html/login.html', (req, res) => {
  res.sendFile(path.join(__dirname, '..', 'html', 'login.html'));
});

// Only allow index.html if logged in
app.get('/html/index.html', (req, res) => {
  if (isLoggedIn) {
    return res.sendFile(path.join(__dirname, '..', 'html', 'index.html'));
  } else {
    return res.redirect('/html/login.html'); // redirect the user back to login.html
  }
});

// Serve everything else normally (All other files such as CSS, JS, images, etc.)
app.use(express.static(path.join(__dirname, '..')));

app.use('/auth-proxy', createProxyMiddleware({
  target: 'http://localhost:4000',
  changeOrigin: true,
  pathRewrite: { '^/auth-proxy': '' }
}));

// Handle login POST
app.post('/login', (req, res) => {
  const res = fetchFn("http://localhost:4000/login", {  // CHANGED: target external auth server
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ email, password })
  });
  console.log("I am here in the log in");
  const { email, password } = req.body;
  if (email === process.env.ADMIN_EMAIL && password === process.env.ADMIN_PASSWORD) {
    isLoggedIn = true; // mark as logged in (only in memory!)
    return res.status(200).json({ success: true });
  }
  return res.status(401).json({ success: false });
});

// Redirect any unknown or unmatched URL to the login page using regular expression (handles 404 fallback)
app.get(/.*/, (req, res) => {
  res.redirect("/html/login.html");
});

// Run the server and listen on port 3001 and also 0.0.0.0 for testing it on mobile phone
app.listen(PORT,'0.0.0.0', () => {
  console.log(`Web server running on port ${PORT}`);
  if (process.argv[2] === "check") {
  console.log("Web server script is reachable and safe.");
  process.exit(0);
}
});
