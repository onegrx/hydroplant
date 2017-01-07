#include "../lib/dht11/dht11.h"
#include "../lib/rgb_color/rgb_color.h"
#include "../lib/fader/fader.h"

dht11 DHT11;

const int dhtPin = 2;
const int moisturePin = A0;
const int lightPin = A1;
const int redPin = 6;
const int greenPin = 5;
const int bluePin = 3;
const int pumpPin = 7;

rgb_color current( 0, 0,  0 );

void setup() {

  Serial.begin(57600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(pumpPin, OUTPUT);

  setColor(0, 0, 0);
}

void loop(){

  fader f (redPin, greenPin, bluePin);

  rgb_color red(255, 0, 0);
  rgb_color blue(0, 0, 255);
  rgb_color green(0, 255, 0);

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

  if(t > 21) {
    digitalWrite(pumpPin, HIGH);
  } else {
    digitalWrite(pumpPin, LOW);
  }

  if(moisture < 350) {
    f.fade(current, green);
    current = green;
  } else if(moisture < 700) {
    f.fade(current, blue);
    current = blue;
  } else {
    f.fade(current, red);
    current = red;
  }


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

void setColor(int red, int green, int blue) {

  //Using common anode
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
