#include "../lib/dht11/dht11.h"
#include "../lib/rgb_color/rgb_color.h"
#include "../lib/fader/fader.h"
#include <Wire.h>
#include <LiquidCrystal.h>

dht11 DHT11;
LiquidCrystal lcd(9, 8, A2, A3, A4, A5);

const int dhtPin = 2;
const int moisturePin = A0;
const int lightPin = A1;
const int redPin = 6;
const int greenPin = 5;
const int bluePin = 3;
const int pumpPin = 7;

rgb_color current(0, 0, 0);

void setup() {

  lcd.begin(16, 2);
  lcd.print("*  Hydroplant  *");
  delay(1000);
  lcd.print("");

  Serial.begin(57600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(pumpPin, OUTPUT);

  setColor(0, 0, 0);
}

void loop(){

  //Initialize colors
  fader f (redPin, greenPin, bluePin);
  rgb_color red(255, 0, 0);
  rgb_color blue(0, 0, 255);
  rgb_color green(0, 255, 0);

  int moisture = analogRead(moisturePin);

  int dhtData = DHT11.read(dhtPin);
  float t = DHT11.temperature;
  float h = DHT11.humidity;

  int light = analogRead(lightPin);

  //Serial print
  Serial.print("Moisture Sensor Value: ");
  Serial.println(moisture);

  Serial.print("Temperature Value: ");
  Serial.println(t);

  Serial.print("Humidity Value: ");
  Serial.println(h);

  Serial.print("Light Value: ");
  Serial.println(light);

  Serial.println();

  printToLcd(t, h, moisture, light);


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

void printToLcd(int t, int h, int m, int l) {
  lcd.setCursor(0,0);
  lcd.print("T: ");
  lcd.print(t);
  lcd.print("  M: ");
  lcd.print(m);
  lcd.setCursor(0,1);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print("  L: ");
  lcd.print(l);
}
