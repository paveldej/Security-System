function toggleAlarm() {
    const armButton = document.getElementById("armButton");
    const systemStatus = document.getElementById("systemStatus");

    if (armButton.textContent === "Arm") {
      armButton.textContent = "Disarm";
      systemStatus.textContent = "Status: Armed";
    } else {
      armButton.textContent = "Arm";
      systemStatus.textContent = "Status: Disarmed";
    }
  }   