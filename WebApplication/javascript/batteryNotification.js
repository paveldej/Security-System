const alertBox = document.getElementById("batteryAlert");
const batteryPercentageElement = document.getElementById("batteryPercentage");

function batteryNotification() {
  console.log("🔋 Battery level is low. Displaying notification...");
  alertBox.classList.remove("hidden");
  alertBox.style.display = "flex";  // Ensure it appears if removed
}

function dismissBatteryAlert() {
  alertBox.classList.add("hidden");

  setTimeout(() => {
    alertBox.style.display = "none";
  }, 600); // matches CSS transition
}

client.on('connect', () => {
  console.log('Connected to MQTT broker for battery monitoring');

  client.subscribe('wioTerminal/battery', (err) => {
    if (err) {
      console.error('Failed to subscribe to battery topic:', err);
    } else {
      console.log('Subscribed to wioTerminal/battery');
    }
  });
});

client.on('message', (topic, message) => {
  if (topic === 'wioTerminal/battery') {
    const batteryLevel = parseInt(message.toString(), 10);
    console.log(`🔋 Battery level received: ${batteryLevel}%`);

    batteryPercentageElement.textContent = `${batteryLevel}%`;

    // Update ring visually
    const circle = document.querySelector('.ring-fill');
    const radius = circle.r.baseVal.value;
    const circumference = 2 * Math.PI * radius;
    const offset = circumference - (batteryLevel / 100) * circumference;

    circle.style.strokeDashoffset = offset;

    // Show alert under 20%
    if (batteryLevel < 20) {
      batteryNotification();
    }
  }
});

// Trigger manually for testing
//document.addEventListener("DOMContentLoaded", () => {
//  setTimeout(() => {
//    batteryNotification();  // You can comment this out later
// }, 2000);
//});
