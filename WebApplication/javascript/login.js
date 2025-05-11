async function login() {
  // these are just to get the input value from the html page.
  const email = document.getElementById("email").value;
  const password = document.getElementById("password").value;
  const warningBox = document.getElementById("funnyWarning");
  const loginError = document.getElementById("loginError");

  const res = await fetch("/login", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ email, password }) // send login data
  });

  const data = await res.json();

  if (res.ok && data.success) {
    // login succeed and the user will be redirected to index.html
    window.location.href = "/html/index.html";
  } else {
    // login failed the user will not be redirected
    loginError.innerText = data.message || "Invalid login"; 
    warningBox.style.display = "block"; // show Pavel warning.
  }
}
