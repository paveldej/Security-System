let logs; 
function GetLogs(logs) {
  if (typeof window === 'undefined') {
    // Node.js environment
    console.log("The button works");
    console.log(logs || "No logs received yet");

    if (logs && logs !== "[]") {
      console.log("LOGS:");
      console.log(logs);
      return logs;
    } else {
      const msg = "No logs available yet.";
      console.log(msg);
      return msg;
    }
  } else {
    // Browser environment
    console.log("The button works");
    console.log(logs || "No logs received yet");

    if (logs && logs !== "[]") {
      const modal = new bootstrap.Modal(document.getElementById('logsModal'));
      const logsContent = document.getElementById('logsContent');
      logsContent.textContent = logs;
      modal.show();
      return logs;
    } else {
      alert("No logs available yet.");
      return "No logs available yet.";
    }
  }
}

// Only run browser-specific logic in browser
if (typeof window !== 'undefined') {
  document.addEventListener("DOMContentLoaded", function () {
    const GetLogsButton = document.getElementById("GetLogs");
    client.on('connect', () => {
      client.subscribe('GetLogs', (err) => {
        if (!err) {
          console.log('Subscribed to GetLogs');
        }
      });
    });

    client.on('message', function (topic, message) {
      if (topic === 'GetLogs') {
        console.log("Logs received: ", message.toString());
        logs = message.toString();
      }
    });

    GetLogsButton.addEventListener('click', function () {
      console.log("Clicked. Logs value:", logs); 
      GetLogs(logs);
    });
  });
}

module.exports = { GetLogs };
