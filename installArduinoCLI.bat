@echo off
mkdir arduino-cli
cd arduino-cli
curl https://downloads.arduino.cc/arduino-cli/arduino-cli_1.1.1_Windows_64bit.zip --output arduino-cli.zip
7z e -y arduino-cli.zip
del arduino-cli.zip
arduino-cli core install arduino:avr