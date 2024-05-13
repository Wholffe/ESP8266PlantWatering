const int relayPin = D4;
const int soilMoistSensorPin = A0;
const int maxSoilMoistValue = 400; //moist
const int minSoilMoistValue = 870; //dry
const int plantmoistureThresholdPercentage = 40;

int currentSoilMoistValue;
int currentSoilMoistPercent;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  currentSoilMoistValue = analogRead(soilMoistSensorPin);
  currentSoilMoistPercent = map(currentSoilMoistValue,minSoilMoistValue,maxSoilMoistValue,0,100);
  Serial.println("SoilMoistValue: " + String(currentSoilMoistValue) + "(" + String(currentSoilMoistPercent) + "%)");
  if (currentSoilMoistPercent < plantmoistureThresholdPercentage){
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
  delay(1000);
}