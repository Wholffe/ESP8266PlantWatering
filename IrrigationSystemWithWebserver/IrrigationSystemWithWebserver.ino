#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Ticker.h>
#include <FS.h>

Ticker timer;

const int relayPin = D4;
const int soilMoistSensorPin = A0;
const int maxSoilMoistValue = 400; //moist
const int minSoilMoistValue = 870; //dry
const int plantmoistureThresholdPercentage = 40;

const char* ssid = "";
const char* password = "";

int currentSoilMoistValue;
int currentSoilMoistPercent;

ESP8266WebServer server;

WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  WiFi.begin(ssid,password);
  while (WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  server.on("/", []() {
    File file = SPIFFS.open("/web/index.html", "r");
    if (!file) {
      server.send(500, "text/plain", "File not found");
      return;
    }
    server.streamFile(file, "text/html");
    file.close();
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketsEvent);

  timer.attach(1, getData);
}

void loop() {
  server.handleClient();
  webSocket.loop();
  handleRelay();
}

void getData(){
  currentSoilMoistValue = analogRead(soilMoistSensorPin);
  currentSoilMoistPercent = map(currentSoilMoistValue,minSoilMoistValue,maxSoilMoistValue,0,100);
  Serial.println(currentSoilMoistPercent);
  String json = "{\"value\":";
  json += currentSoilMoistPercent;
  json += "}";
  webSocket.broadcastTXT(json.c_str(),json.length());
}

void handleRelay(){
  if (currentSoilMoistPercent < plantmoistureThresholdPercentage){
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
}

void webSocketsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){

}