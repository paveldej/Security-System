async function sendLoginRequest({ email, password }, fetchFn = fetch) {
  const res = await fetchFn("http://localhost:3001/login", {  // CHANGED: target external auth server
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ email, password })
  });

  const data = await res.json();

  return { status: res.status, success: data.success, message: data.message };
}

// for the login button UI
async function login() {
  const email = document.getElementById("email").value;
  const password = document.getElementById("password").value;
  const warningBox = document.getElementById("funnyWarning");
  const loginError = document.getElementById("loginError");

  const result = await sendLoginRequest({ email, password });

  if (result.success) {

    window.location.href = "/html/index.html";
    console.log("successfully loggedin");
  } else {
    loginError.innerText = result.message || "The Email Or Password Provided is Incorrect!";
    warningBox.style.display = "block";
  }
}

module.exports = { sendLoginRequest }; // For testing
