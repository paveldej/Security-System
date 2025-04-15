

  async function allowNotifications() {
    let permission = await Notification.requestPermission();
    
    if (permission === 'granted') {
      batteryNotification();
    }
  }

allowNotifications();
batteryNotification();