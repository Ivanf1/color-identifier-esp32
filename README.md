# Async web server with ESP32 and color sensor

Read from a color sensor and send the value to a web application.

## How it works

The server exposes an API from wich to get a single color reading and a websocket to get multiple color readings in a short time. The value that get sended to the client is in HEX.

The color sensor is a TCS34725. It uses the I2C Communication Protocol so it just needs two pins for the connection to the microcontroller.

## Wiring

- LED pin of color sensor to pin 26 of ESP
- SDA pin of color sensor to pin 21 of ESP (21 is the pin for sda on my version of the esp32, it may be different on other versions)
- SCL pin of color sensor to pin 22 of ESP (22 is the pin for scl on my version of the esp32, it may be different on other versions)
- 3V3 pin of color sensor to 3V3 pin of ESP
- GND pin of color sensor to GND pin of ESP

## Front-end for this application

[Color identifyer][1]

### Todo list

- Better reading speed from the color sensor
- Better tuning for accuracy improvement

[1]: https://github.com/Ivanf1/color-identifier
