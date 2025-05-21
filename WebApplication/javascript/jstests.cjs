require('dotenv').config();
const { sendLoginRequest } = require('./login.js');
const { getLogs } = require('./getLogs.js');

function mockFetch(success) {
  return async () => ({
    json: async () => ({ success, message: success ? undefined : "Invalid credentials" }),
    status: success ? 200 : 401,
    ok: success
  });
}

(async () => {
  const res1 = await sendLoginRequest({ email: process.env.ADMIN_EMAIL, password: process.env.ADMIN_PASSWORD}, mockFetch(true));
  console.assert(res1.success === true, "Test Passed: Valid credentials");

  const res2 = await sendLoginRequest({ email: "wrong", password: "wrong" }, mockFetch(false));
  console.assert(res2.success === false, "Test failed: Invalid credentials");

  let logs = '[{"timestamp":"12/05/2025 21:22:00","Trigger":"Intruder Detected"},{"timestamp":"12/05/2025 21:22:58","Trigger":"Intruder Detected"},{"timestamp":"12/05/2025 21:40:46","Trigger":"Intruder Detected"}]';
  const res3 = getLogs(logs);
  console.assert(typeof res3 === 'string' && res3.includes("Intruder"), "❌ Test failed: Logs output not as expected");

  console.log("Running getLogs test with empty logs");
  const res4 = getLogs(null);
  console.assert(typeof res4 === 'string' && res4.includes("No logs"), "❌ Test failed: Should handle empty logs");
  console.log("All login tests passed.");
})();