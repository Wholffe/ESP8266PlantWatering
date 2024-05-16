#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Ticker.h>

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

char webpage[] PROGMEM = R"=====(
<html>
<!-- Adding a data chart using Chart.js -->
<head>
  <script src='https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js'></script>
</head>
<body onload="javascript:init()">
<!-- Adding a slider for controlling data rate -->
<div>
  <canvas id="line-chart" width="800" height="450"></canvas>
</div>
<!-- Adding a websocket to the client (webpage) -->
<script>
  var webSocket, dataPlot;
  var maxDataPoints = 20;
  
  function removeData(){
    dataPlot.data.labels.shift();
    dataPlot.data.datasets[0].data.shift();
  }

  function addData(label, data) {
    if(dataPlot.data.labels.length > maxDataPoints) removeData();
    dataPlot.data.labels.push(label);
    dataPlot.data.datasets[0].data.push(data);
    dataPlot.update();
  }

  function init() {
    webSocket = new WebSocket('ws://' + window.location.hostname + ':81/');
    dataPlot = new Chart(document.getElementById("line-chart"), {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          data: [],
          label: "Soil Moisture (%)",
          borderColor: "#76300f",
          fill: false
        }]
      }
    });

    webSocket.onmessage = function(event) {
      var data = JSON.parse(event.data);
      var today = new Date();
      var t = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
      addData(t, data.value);
    }
  }
</script>
</body>
</html>
)=====";

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  WiFi.begin(ssid,password);
  while (WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Adress: ");
  Serial.println(WiFi.localIP());

  server.on("/",[](){
    server.send_P(200,"text/html",webpage);
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketsEvent);

  timer.attach(1,getData);
}

void loop() {
  server.handleClient();
  webSocket.loop();
  handleRelais();
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

void handleRelais(){
  if (currentSoilMoistPercent < plantmoistureThresholdPercentage){
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
}

void webSocketsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){

}