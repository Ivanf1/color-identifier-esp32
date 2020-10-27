#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_TCS34725.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <secrets.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
const int colorSensorLedPin = 26;
boolean isColorSensorOn = false;

uint32_t clientID = 0;
boolean doStream = false;
AsyncWebServer server(80);
AsyncWebSocket ws("/colorStream");

void readColorSensor(char * hex) {
  float red, green, blue;

  tcs.getRGB(&red, &green, &blue);
  // Serial.print("R: "); Serial.print(int(red));
  // Serial.print("\tG: "); Serial.print(int(green));
  // Serial.print("\tB: "); Serial.print(int(blue));

  // Serial.print("\tHEX: ");
  // Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  // Serial.println();

  char r[12], g[12], b[12];
  sprintf(r, "%x", (int)red);
  sprintf(g, "%x", (int)green);
  sprintf(b, "%x", (int)blue);
  strcpy(hex, r);
  strcat(hex, g);
  strcat(hex, b);
}

void toggleColorSensor(boolean enable) {
  enable ? tcs.enable() : tcs.disable();
  isColorSensorOn = enable;
  digitalWrite(colorSensorLedPin, enable);
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_DATA) {
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len) {
      //the whole message is in a single frame and we got all of its data
      if(info->opcode == WS_TEXT) {
        data[len] = 0;
        if(strcmp((char *)data, "startColorStream") == 0) {
          toggleColorSensor(true);
          clientID = client->id();
          doStream = true;
        } else if(strcmp((char *)data, "stopColorStream") == 0) {
          toggleColorSensor(false);
          doStream = false;
        }
      }
    }
  } else if(type == WS_EVT_CONNECT) {
    Serial.println("Websocket client connection received");
  } else if(type == WS_EVT_DISCONNECT) {
    clientID = 0;
    doStream = false;
    Serial.println("Client disconnected");
  }
}

void setup(void) {
  Serial.begin(115200);

  // Color sensor setup ------
  if (tcs.begin()) {
    pinMode(colorSensorLedPin, OUTPUT);
    Serial.println("Found sensor");
    toggleColorSensor(false);
  } else {
    Serial.println("No TCS34725 found");
    return;
  }
  // -------------------------

  // Server setup ------------
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  } else {
    Serial.println("WiFi connected\n");
  }
  Serial.println(WiFi.localIP());

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.on("/", [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = new AsyncWebServerResponse();
    if(!doStream) {
      if(!isColorSensorOn) toggleColorSensor(true);
      char hex[36];
      readColorSensor(hex);
      response = request->beginResponse(200, "text/plain", (char *)hex);
      toggleColorSensor(false);
    } else {
      response = request->beginResponse(503, "text/plain", "busy");
    }
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    return;
  });

  server.begin();
  // -------------------------
}

void loop(void) {
  if(clientID != 0 && doStream) {
    try {
      char hex[36];
      readColorSensor(hex);
      if(clientID != 0 && doStream) ws.client(clientID)->text((char *)hex);
    } catch(...) {
      Serial.println("error when sendig to ws client");
    }
  }
}