#include "../../lib/dht11/dht11.h"
#include "../../lib/rgb_color/rgb_color.h"
#include "../../lib/fader/fader.h"
#include <Wire.h>
#include <LiquidCrystal.h>
#include <dht11.h>
#include <time.h>

dht11 DHT11;
LiquidCrystal lcd(9, 8, A2, A3, A4, A5);

const int dhtPin = 2;
const int moisturePin = A0;
const int lightPin = A1;
const int thresholdMoisture = 900;
const int minMoisture = 500;
const int timeout = 1000;           //todo change to an hour
const int wateringTimeout = 100;    //todo change to a minute


char *getCurrentTime();

void waterOneUnit();

int readMoisture() ;
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

void loop() {

  //Initialize colors
  fader f (redPin, greenPin, bluePin);
  rgb_color red(255, 0, 0);
  rgb_color blue(0, 0, 255);
  rgb_color green(0, 255, 0);

  int moisture = analogRead(moisturePin);

    int currentMoisture = readMoisture();
    Serial.print("current moisture: ");
    Serial.println(currentMoisture);

    if (currentMoisture > thresholdMoisture) {
        Serial.println("i'm inside if");

        do {
            Serial.print("water time: ");
            Serial.println(millis());

            waterOneUnit();

            delay(wateringTimeout);
            currentMoisture = readMoisture();

        } while (currentMoisture > minMoisture);
    }
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
//    const int numberOfMoistureSamples = 15;
//    const int moistureSamplingTimeout = 1000;
//    int moistureSamplesSum = 0;
//    for (int i = 0; i < numberOfMoistureSamples; i++) {
//        moistureSamplesSum += (int) analogRead(moisturePin);
//        delay(moistureSamplingTimeout);
//    }
//    return moistureSamplesSum / numberOfMoistureSamples;
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
