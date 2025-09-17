#include <WiFi.h>
#include <LITTLEFS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

const char* ssid = "DEIN_SSID";
const char* password = "DEIN_PASSWORT";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

JsonDocument json;

void sendLogLine(const String& line) {
  json["logline"] = line;
  String out;
  serializeJson(json, out);
  ws.textAll(out);
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      String msg = String((char*)data);
      JsonDocument doc;
      DeserializationError err = deserializeJson(doc, msg);
      if (!err && doc["cmd"]["action"] == "subscribe" && doc["cmd"]["subaction"] == "log_data") {
        doc["logline"] = "Webserial subscribe erfolgreich!";
        json = doc; // copy local doc to global json object for later use
        client->text(doc.as<String>());
        // Logdaten zyklisch senden (z.B. per Timer oder Task)
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi verbunden: " + WiFi.localIP().toString());

  server.serveStatic("/", LittleFS, "/").setDefaultFile("webserial_frame.html");
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();

  xTaskCreate(
    [](void*) {
      int counter = 0;
      while (true) {
        String line = "Logzeile " + String(++counter);
        sendLogLine(line);
        delay(2000);
      }
    },
    "LogTask", 4096, nullptr, 1, nullptr
  );
}

void loop() {
  // ...existing code...
}