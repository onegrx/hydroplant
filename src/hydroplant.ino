#include <dht11.h>

dht11 DHT11;

const int dhtPin = 2;
const int moisturePin = A0;
const int lightPin = A1;

void setup(){
  Serial.begin(57600);
}

void loop(){

  int moisture = analogRead(moisturePin);

  int dhtData = DHT11.read(dhtPin);
  float t = DHT11.temperature;
  float h = DHT11.humidity;

  int light = analogRead(lightPin);

  Serial.print("Moisture Sensor Value: ");
  Serial.println(moisture);

  Serial.print("Temperature Value: ");
  Serial.println(t);

  Serial.print("Humidity Value: ");
  Serial.println(h);

  Serial.print("Light Value: ");
  Serial.println(light);

  Serial.println();

  delay(1000);
}

int readTemperature() {
  int dhtData = DHT11.read(dhtPin);
  return (int) DHT11.temperature;
}

int readHumidity() {
  int dhtData = DHT11.read(dhtPin);
  return (int) DHT11.humidity;
}

int readMoisture() {
  return (int) analogRead(moisturePin);
}

int readLight() {
  return (int) analogRead(lightPin);
}
