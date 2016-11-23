#include <dht11.h>

dht11 DHT11;
#define dataPin 2

void setup(){
  Serial.begin(57600);
}

void loop(){

  int moisture = analogRead(A0);

  int dhtData = DHT11.read(dataPin);
  float t = DHT11.temperature;
  float h = DHT11.humidity;

  Serial.print("Moisture Sensor Value: ");
  Serial.println(moisture);

  Serial.print("Temperature Value: ");
  Serial.println(t);

  Serial.print("Humidity Value: ");
  Serial.println(h);

  Serial.println();

  delay(1000);
}
