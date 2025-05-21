let logs = [];


function getLogMessage(key, value) { //method to choose what gets added to log
  if (key === 'Status' && value === 'Armed') {
    return 'Alarm was armed!';
  }
  if (key === 'Status' && value === 'Disarmed') {
    return 'Alarm was disarmed!';
  }
  if (key === 'Trigger' && value === 'Intruder Detected') {
    return 'Alarm was triggered!';
  }
  

  // Default fallback
  return `${key}:${value}`;
}

function getLogs(logs) {
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

      let formattedLogs = logs.slice().map(log => {
        try {
          const parsed = JSON.parse(log);
          const timestamp = parsed.timestamp || "No timestamp";
      
          delete parsed.timestamp;
          const [key, value] = Object.entries(parsed)[0] || ["<unknown>", "<unknown>"];
          const logMessage = getLogMessage(key, value);
          return `${logMessage}, ${timestamp}`;
        } catch (e) {
          return log.toString();
        }
      }).join('\n');
      
      logsContent.textContent = formattedLogs;

      modal.show();
      return logs;
    } else {
      console.log("No logs available yet."); // always safe fallback

// Optionally display a Bootstrap modal with a simple message, e.g.:

const logsModal = new bootstrap.Modal(document.getElementById('logsModal'));
const logsContent = document.getElementById('logsContent');

logsContent.textContent = "No logs available yet.";
logsModal.show();
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
    const getLogsButton = document.getElementById("getLogs");

    // Ensure logs are cleared each time the button is clicked
    getLogsButton.addEventListener('click', function () {
      logs = [];
      sendlogRequest();

      setTimeout(() => {
        console.log("Clicked. Logs value:", logs);
        getLogs(logs);
      }, 1000); 
    });

    client.on('connect', () => {
      client.subscribe('getLogs', (err) => {
        if (!err) {
          console.log('Subscribed to getLogs');
        }
      });
    });

    client.on('message', function (topic, message) {
      if (topic === 'getLogs') {
        const logStr = message.toString();
        console.log("Logs received: ", logStr);
        logs.push(logStr);
      }
    });
  });
}

module.exports = { getLogs };
