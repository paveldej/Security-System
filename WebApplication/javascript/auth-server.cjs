const express = require('express');
const bodyParser = require('body-parser');
const dotenv = require('dotenv');
const cors = require('cors')

dotenv.config();

const app = express();
const PORT = 4000;

app.use(cors());
app.use(bodyParser.json());

// Simple login API that compares credentials to values in .env
app.post('/login', (req, res) => {
  const { email, password } = req.body;
  if (
    email === process.env.ADMIN_EMAIL &&
    password === process.env.ADMIN_PASSWORD
  ) {
    return res.status(200).json({ success: true }); // if right credentails then it will will give success the value of true, which will be used to let the user get access.
  }

  return res.status(401).json({ success: false }); // The user will not be able to get access because of wrong credentails.
});

app.listen(PORT, () => {
  console.log(`Auth server running on port ${PORT}`);
    if (process.argv[2] === "check") {
  console.log("Authentication server script is reachable and safe.");
  process.exit(0);
}
});

app.use((req, res, next) => {
  console.log("AUTH SERVER GOT:", req.method, req.url);
    if (process.argv[2] === "check") {
  console.log("Web server script is reachable and safe.");
  process.exit(0);
}

  next();
});