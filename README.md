## Getting started

This documentation is meant to teach you how to set up this security system. Follow the instructions carefully, as they are essential for the functional integrity of your system.

# Connecting to a power source

In order to use the terminal you have to connect it to some power source.
For ease of use, we have provided a battery that you can to connect using the pins on the front of the battery/chassis to the back of the terminal.

You can use a USB-C to power either the terminal directly or to charge the battery. To turn the battery on, hold down the black button on the bottom of the battery.

# Connecting the sensors

Once you're connected the battery/chassis you have to plug in the sensors.

The "Grove Ultrasonic Ranger" (two tubular metal pieces) is responsible for detecting motions. It should be connected to the port opposite to the three-way power on button (on the battery/chassis).

The "Grove RGB Chainable LED" should be connected directly below the power on button. Make sure you plug it in to the "in" part of the LED.

# How to deploy

Now that you've connected the sensors and provided power, you can now turn the device on. Put it on middle position.

First you have to run the server.js file inside the Web Application/javascript folder in your terminal. Once you've run the command, you must leave the terminal open to use the application. Keep in mind you must have Node.js downloaded. (we need to add which dependencies to add).

After this, open the Web Application folder and click on the index.html file inside the html folder there.

Now you can arm and disarm your security system and get notification on the application and by email about potential intruders and information about the state of your battery.
