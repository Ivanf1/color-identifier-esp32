# Async web server with ESP32 and color sensor

The server has an endpoint to get a single color reading and a websocket to get multiple color readings in a short time.

The color sensor is a TCS34725

## Wiring

LED pin of color sensor to pin 26 of ESP
SDA pin of color sensor to pin 21 of ESP (21 is the pin for sda on my version of the esp32, it may be different on other versions)
SCL pin of color sensor to pin 22 of ESP (22 is the pin for scl on my version of the esp32, it may be different on other versions)
3V3 pin of color sensor to 3V3 pin of ESP
GND pin of color sensor to GND pin of ESP
