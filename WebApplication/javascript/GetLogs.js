document.addEventListener("DOMContentLoaded", function () {
  const GetLogsButton = document.getElementById("GetLogs");
  let logs;

  // Subscribe to the logs topic
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
      logs = message.toString();  // Store the received logs
    }
  });

  // Function to display logs in a modal popup
  function GetLogs() {
    console.log("The button works");
    console.log(logs || "No logs received yet");

    // Check if logs are available
    if (logs) {
      // Get the modal and logs content container
      const modal = new bootstrap.Modal(document.getElementById('logsModal'));
      const logsContent = document.getElementById('logsContent');

      // Set the logs content inside the modal
      logsContent.textContent = logs;  // Insert the logs string into the modal

      // Show the modal
      modal.show();
    } else {
      alert("No logs available yet.");
    }
  }

  // Manually close the modal when Close button is clicked


  // Attach event listener to the button
  GetLogsButton.addEventListener('click', GetLogs);
});
