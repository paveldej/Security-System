async function login() {
    const email = document.getElementById("email").value;
    const password = document.getElementById("password").value;
    const warningBox = document.getElementById("funnyWarning");

    const res = await fetch("http://localhost:3000/login", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ email, password })
    });
  
    const data = await res.json();
  
    if (data.success) {
      window.location.href = "../html/index.html"; // redirect on success
    } else {
      document.getElementById("loginError").innerText = data.message;
      warningBox.style.display = "block"; // Show funny warning
    }
  }
  
  