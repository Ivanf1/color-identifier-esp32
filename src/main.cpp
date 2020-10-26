#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <secrets.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

AsyncWebServer server(80);
AsyncWebSocket ws("/color");

void readColorSensor(char * hex) {
  float red, green, blue;

  tcs.getRGB(&red, &green, &blue);
  Serial.print("R: "); Serial.print(int(red));
  Serial.print("\tG: "); Serial.print(int(green));
  Serial.print("\tB: "); Serial.print(int(blue));

  Serial.print("\tHEX: ");
  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.println();

  char r[12], g[12], b[12];
  sprintf(r, "%x", (int)red);
  sprintf(g, "%x", (int)green);
  sprintf(b, "%x", (int)blue);
  strcpy(hex, r);
  strcat(hex, g);
  strcat(hex, b);
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT) {
    Serial.println("Websocket client connection received");
  } else if(type == WS_EVT_DISCONNECT) {
    Serial.println("Client disconnected");
  } else if(type == WS_EVT_DATA) {
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len) {
      //the whole message is in a single frame and we got all of it's data
      if(info->opcode == WS_TEXT) {
        data[len] = 0;
        if(strcmp((char *)data, "getSensorColor") == 0) {
          char hex[36];
          readColorSensor(hex);
          client->text((char *)hex);
        }
        Serial.printf("Message from client: %s\n", (char*)data);
      }
    }
  }
}

void setup(void) {
  Serial.begin(115200);

  // Color sensor setup ------
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
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
    Serial.println("wifi connected\n");
  }

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  Serial.println(WiFi.localIP());
  server.begin();
  // -------------------------
}

void loop(void) {}