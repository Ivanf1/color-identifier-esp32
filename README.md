# Async web server with ESP32 and color sensor

Read from a color sensor and send the value to a web application.

## How it works

The server exposes an API from wich to get a single color reading and a websocket to get multiple color readings in a short time. The value that get sended to the client is in HEX.

The color sensor is a TCS34725. It uses the I2C Communication Protocol so it just needs two pins for the connection to the microcontroller.

## Wiring

![alt text][wiring]

[wiring]: https://github.com/Ivanf1/color-identifier-esp32/blob/master/images/wiring.jpg?raw=true "Wiring"

## Front-end for this application

[Color identifier][1]

### Todo list

- Better reading speed from the color sensor
- Better tuning for accuracy improvement

[1]: https://github.com/Ivanf1/color-identifier
