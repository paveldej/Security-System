function toggleAlarm() {
  const armButton = document.getElementById("armButton");
  const systemStatus = document.getElementById("systemStatus");
  const indicator = document.getElementById("armedIndicator");

  if (armButton.textContent === "Arm") {
    armButton.textContent = "Disarm";
    systemStatus.textContent = "Status: Armed";
    indicator.classList.add("armed");
  } else {
    armButton.textContent = "Arm";
    systemStatus.textContent = "Status: Disarmed";
    indicator.classList.remove("armed");
  }
}
