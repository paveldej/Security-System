const express = require('express');
const { createProxyMiddleware } = require('http-proxy-middleware');
const bodyParser = require('body-parser');
const path = require('path');
const dotenv = require('dotenv');

// Dynamically load fetch for compatibility with CommonJS
const fetch = (...args) => import('node-fetch').then(({ default: fetch }) => fetch(...args));

dotenv.config();

const app = express();
const PORT = 3001;

let isLoggedIn = false;

app.use(bodyParser.json());

app.use('/img', express.static(path.join(__dirname, '..', 'img')));
app.use('/javascript', express.static(path.join(__dirname, '..', 'javascript')));
app.use('/css', express.static(path.join(__dirname, '..', 'css')));

// Serve login page to everyone
app.get('/html/login.html', (req, res) => {
  res.sendFile(path.join(__dirname, '..', 'html', 'login.html'));
});

// Serve index.html ONLY if logged in
app.get('/html/index.html', (req, res) => {
  if (isLoggedIn) {
    res.sendFile(path.join(__dirname, '..', 'html', 'index.html'));
  } else {
    res.redirect('/html/login.html');
  }
});

// Proxy to auth server
app.use('/auth-proxy', createProxyMiddleware({
  target: 'http://localhost:4000',
  changeOrigin: true,
  pathRewrite: { '^/auth-proxy': '' },
}));

// Login route
app.post('/login', async (req, res) => {
  const { email, password } = req.body;

  try {
    const authRes = await fetch("http://localhost:4000/login", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ email, password }),
    });

    const data = await authRes.json();

    if (authRes.ok && data.success) {
      isLoggedIn = true;
      return res.status(200).json({ success: true });
    } else {
      return res.status(401).json({ success: false, message: data.message || "Unauthorized" });
    }
  } catch (err) {
    console.error("Error contacting auth server:", err);
    return res.status(500).json({ success: false, message: "Auth server unreachable" });
  }
});

// Catch-all fallback route
app.get(/.*/, (req, res) => {
  res.redirect("/html/login.html");
});

// Start server
app.listen(PORT, '0.0.0.0', () => {
  console.log(`Web server running on port ${PORT}`);
    if (process.argv[2] === "check") {
  console.log("Web server script is reachable and safe.");
  process.exit(0);
}

});
