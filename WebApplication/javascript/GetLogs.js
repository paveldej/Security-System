let logs = [];

function GetLogs(logs) {
  if (typeof window === 'undefined') {
    // Node.js
    console.log("The button works");
    console.log(logs || "No logs received yet");
    if (logs && logs.length > 0) {
      console.log("LOGS:");
      console.log(logs);
      return logs;
    } else {
      const msg = "No logs available yet.";
      console.log(msg);
      return msg;
    }
  } else {
    // Browser
    console.log("The button works");
    console.log(logs || "No logs received yet");

    if (logs && logs.length > 0) {
      const modal = new bootstrap.Modal(document.getElementById('logsModal'));
      const logsContent = document.getElementById('logsContent');

      let formattedLogs = logs.map(log => {
        try {
          return JSON.stringify(JSON.parse(log), null, 2);
        } catch (e) {
          return log.toString();
        }
      }).join('<hr>');

      logsContent.innerHTML = `<pre>${formattedLogs}</pre>`;
      modal.show();
      return logs;
    } else {
      alert("No logs available yet.");
      return "No logs available yet.";
    }
  }
}

function sendlogRequest() {
  client.publish('requestLogs', 'getLogs');
}

// Browser setup
if (typeof window !== 'undefined') {
  document.addEventListener("DOMContentLoaded", function () {
    const GetLogsButton = document.getElementById("GetLogs");

    // Ensure logs are cleared each time the button is clicked
    GetLogsButton.addEventListener('click', function () {
      logs = [];
      sendlogRequest();

      setTimeout(() => {
        console.log("Clicked. Logs value:", logs);
        GetLogs(logs);
      }, 1000); 
    });

    client.on('connect', () => {
      client.subscribe('GetLogs', (err) => {
        if (!err) {
          console.log('Subscribed to GetLogs');
        }
      });
    });

    client.on('message', function (topic, message) {
      if (topic === 'GetLogs') {
        const logStr = message.toString();
        console.log("Logs received: ", logStr);
        logs.push(logStr);
      }
    });
  });
}

module.exports = { GetLogs };
