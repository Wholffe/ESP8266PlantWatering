int relayPin = D4;
int soilMoistSensorPin = A0;
int currentSoilMoistValue;
int currentSoilMoistPercent;

int maxSoilMoistValue = 400; //moist
int minSoilMoistValue = 870; //dry
int plantmoistureThresholdPercentage = 40;

void setup() {
  Serial.begin(115200);
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  currentSoilMoistValue = analogRead(soilMoistSensorPin);
  currentSoilMoistPercent = map(currentSoilMoistValue,minSoilMoistValue,maxSoilMoistValue,0,100);
  Serial.print("Current soil moist value: ");
  Serial.println(String(currentSoilMoistValue));
  Serial.print("Current soil most value in percent: ");
  Serial.println(String(currentSoilMoistPercent));
  if (currentSoilMoistPercent < plantmoistureThresholdPercentage){
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
  delay(1000);
}
