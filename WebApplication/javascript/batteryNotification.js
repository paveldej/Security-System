function batteryNotification() {
  const alert = document.getElementById("batteryAlert");
  alert.classList.add("show");
}

function dismissBatteryAlert() {
  document.getElementById("batteryAlert").classList.remove("show");
}
