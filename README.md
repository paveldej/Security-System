# Pavel's Security System 

## Table of Contents

- [System description](#system-description)
- [Purpose and benefits](#purpose-and-benefits-of-the-system)
- [System Architecture](#system-architecture)
- [Contributions](#contributions)
- [Tools & Hardware used](#tools--hardware-used)
- [Setup Manual](#getting-started)  
    - [Installing dependencies](#installing-dependencies)
    - [Uploading the source code](#uploading-the-source-code)
    - [Connecting to a power source](#connecting-to-a-power-source)
    - [Connecting the sensors](#connecting-the-sensors)
- [License](#lincense)

## System Description

Pavel’s security system intends to prevent break ins by notifying users about suspicious activities outside their doors. 
It provides an application where users can arm, disarm and manually trigger the system. 
Additionally, users can receive notifications and check the device’s history of events.
Pavel’s security system makes use of an Arduino based Terminal, called the Wio Terminal by Seeed Studio. 
The system provides two means of interaction for users; it has a web application and a display through the Wio Terminal. 

## Purpose and benefits of the system

Pavels Security System is a simple and reliable IoT solution to increase security for homes of our customers, which is relatively easy-to-use for regular people. 
Since the System provides the authentication mechanism, it lets users have multiple devices in different places, and control all of them through different accounts, which are provided to users from the box.
For the user’s convenience, the Seeed Wio-Terminal provides a UI, that allows users to interact with the system and perform necessary actions, such as connecting to the wifi network and arm/disarm the system.


## System Architecture 
Pavel’s security system is composed of 3 layers; the web application & authentication layer, the broker layer and the device software & email server layer. Each layer has its own software components and serves a unique purpose.

The web application & authentication layer provides the user with a visual interface after and only after they have been authenticated by the authentication server and static file server.

The device software & email server layer handles reading and processing input data, sending email notifications and displaying system info on the Wio terminal.

Lastly, the broker layer handles communication between the other two layers with a pub/sub model.

*Diagram of the Architecture:*
![Architecture Diagram](docs/architecture_diagram.svg)


## Setup Manual 🛠️

### Installing dependencies 

⚠️
Make sure you have [Arduino CLI](https://docs.arduino.cc/arduino-cli/installation/) installed.

### Uploading the source code 

Once you have all the dependencies listed, you have to compile the code
<pre><code>arduino-cli compile --fqbn Seeeduino:samd:seeed_wio_terminal</code></pre>

After doing this, you have to upload it to the terminal using the following command:
<pre><code>arduino-cli upload -t < path to bin > -b Seeeduino:samd:seeed_wio_terminal -p < port > </code></pre>

### Connecting to a power source

In order to use the terminal you have to connect it to some power source.
For ease of use, we have provided a battery that you can to connect using the pins on the front of the battery/chassis to the back of the terminal.

You can use a USB-C to power either the terminal directly or to charge the battery. To turn the battery on, hold down the black button on the bottom of the battery.

### Connecting the sensors

Once you've connected the battery/chassis you have to plug in the sensors.

The "Grove Ultrasonic Ranger" (two tubular metal pieces) is responsible for detecting motions. It should be connected to the port opposite to the three-way power on button (on the battery/chassis).

The "Grove RGB Chainable LED" should be connected directly below the power on button. Make sure you plug it in to the "in" part of the LED.

### How to deploy

Now that you've connected the sensors and provided power, you can now turn the device on. Put it on middle position.

First you have to download Node.js and import the following dependencies using npm:
- express
- nodemailer
- cors
- dotenv
- mqtt

Now you can run the "node server.cjs" file inside the Web Application/javascript folder through your terminal. Once you've run the command, you must leave the terminal open to use the application. Keep in mind you must have Node.js downloaded, and all the dependencies imported.

After this, open the Web Application folder and click on the index.html file inside the html folder there.

Now you can arm and disarm your security system and get notification on the application and by email about potential intruders and information about the state of your battery.

## Contributions

- **Pavel Dejkoski** (@dejkoski)- CI/CD pipeline, Email notification server, Logs(storing and sending trough mqtt), Arduino UI, Website Notifications, Arduino and Web Page MQTT connection
- **Simon Nilrud** (@Ovan81, @nilrud)- Web Page frontend, Log display, Manual triggering functionality, sound-distance weight based triggering 
- **Volodymyr Filippov** (@volfil) - Arduino UI & Network connection, Arduino UI
- **Riad Santir** (@santir) - Web Page authentication server and frontend, 
- **Máté Stier** (@mates) - CI/CD pipeline, Arduino sensor and actuator logic, Arduino MQTT connection, Arduino UI


## Tools & Hardware used 

- [Wio terminal](https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started/)
- [Battery Chassis](https://wiki.seeedstudio.com/Wio-Terminal-Chassis-Battery_650mAh/)
- [Ultrasonic Ranger](https://wiki.seeedstudio.com/Grove-Ultrasonic_Ranger/)
- [Chainable RGB LED](https://wiki.seeedstudio.com/Grove-Chainable_RGB_LED/)
- [Buzzer](https://wiki.seeedstudio.com/Grove-Buzzer/)
- [SD card (min. 2GB)](https://en.wikipedia.org/wiki/SD_card)

## Lincense 📜

This project is licensed under the [GNU General Public License v3.0](LICENSE).
