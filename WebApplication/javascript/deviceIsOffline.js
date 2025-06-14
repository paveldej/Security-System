const deviceIsOfflineBox = document.getElementById("offlineOverlay");

let isStatusReceived = false;

setInterval(deviceIsOffline, 5000);

function deviceIsOffline() {
    client.publish('Status/setStatus', 'status');
    setTimeout(() => {
        if (!isStatusReceived) {
            deviceIsOfflineBox.classList.remove("hidden");
        }
        isStatusReceived = false;
    }, 2000);
}


client.on('message', function (topic, message) {
  if (topic === "Status/getStatus") {
    isStatusReceived = true;
    deviceIsOfflineBox.classList.add("hidden");
  }
});


function reEstablishConnection() {
    deviceIsOfflineBox.classList.add("hidden");
    location.reload();
}