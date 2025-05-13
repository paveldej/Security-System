#!/bin/sh

cd ~


# installing the arduino-cli
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh   

export PATH=$PATH:/root/bin
arduino-cli -version

# creating a config file for the arduino board
echo "board_manager:
  additional_urls:
    - https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json" > .arduino-cli.yaml

arduino-cli core update-index --config-file .arduino-cli.yaml
arduino-cli core install Seeeduino:samd --config-file .arduino-cli.yaml

# Here we add all the dependencies used in arduino code

arduino-cli lib install "Grove Ultrasonic Ranger@1.0.1"
arduino-cli lib install "PubSubClient@2.8"
arduino-cli lib install "Seeed Arduino rpcWifi@1.1.0"
arduino-cli lib install "SparkFun BQ27441 LiPo Fuel Gauge Arduino Library@1.1.0"
arduino-cli lib install "Seeed Arduino rpcWifi@1.1.0"
arduino-cli lib install "ArduinoJson@7.4.1"
arduino-cli lib install "NTPClient@3.2.1"
arduino-cli lib install "Time@1.6.1"

# We have to manually add the Grove ChainableLED library from a .zip because it doesn't work otherwise

mkdir -p ~/Arduino/libraries

wget -O ~/Arduino/libraries/Grove_Chainable_RGB_LED-master.zip https://github.com/Seeed-Studio/Grove_Chainable_RGB_LED/archive/refs/heads/master.zip

unzip ~/Arduino/libraries/Grove_Chainable_RGB_LED-master -d ~/Arduino/libraries/


cd -

