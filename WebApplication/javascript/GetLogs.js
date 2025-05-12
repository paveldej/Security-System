document.addEventListener("DOMContentLoaded", function () {
  const GetLogsButton = document.getElementById("GetLogs");
  let logs;

  client.on('connect', () => {
    client.subscribe('GetLogs', (err) => {
      if (!err) {
        console.log('Subscribed to GetLogs');
      }
    });
  });

  client.on('message', function (topic, message) {
    if (topic === 'GetLogs') {
      logs = message.toString();
    }
  });

  function GetLogs() {
    console.log("The button works");
    console.log(logs || "No logs received yet");
  }

  GetLogsButton.addEventListener('click', GetLogs);
});
