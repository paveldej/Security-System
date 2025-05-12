require('dotenv').config();
const { sendLoginRequest } = require('./login.js');

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

  console.log("All login tests passed.");
})();