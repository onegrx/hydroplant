#include "../../lib/dht11/dht11.h"
#include "../../lib/rgb_color/rgb_color.h"
#include "../../lib/fader/fader.h"
#include <Wire.h>
#include <LiquidCrystal.h>
#include <dht11.h>
#include <time.h>
#include "../../lib/plant_rgb/PlantStateColorCalculator.h"

dht11 DHT11;
LiquidCrystal lcd(9, 8, A2, A3, A4, A5);

// constants
const int dhtPin = 2;
const int moisturePin = A0;
const int lightPin = A1;
const int maxMoisture = 900;
const int minMoisture = 500;
const int nightThresholdLight = nullptr; // TODO
const int wateringUnit = 10000; //in seconds
const int timeout = 500;
const int wateringTimeout = 60000;
const int redPin = 6;
const int greenPin = 5;
const int bluePin = 3;
const int pumpPin = 7;

rgb_color currentColor(0, 0, 0);

// structures
struct Watering {
    bool waterTime = false;
    unsigned int start;
    bool isNow = false;
} watering;

struct Measure {
    int temperature;
    int humidity;
    int moisture;
    int night;
    unsigned int time;
} measure;

// functions
unsigned int getCurrentTime();

void waterPlant();

void putWaterAway();

int readMoisture();

void setColor(int red, int green, int blue);

void printToLcd(Measure measurements);

int readLight();

void updateMeasurements();

bool hasTimePassed(int startTime, int interval);

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

    // plant measurement part
    updateMeasurements();

    printToLcd(measure);

    PlantStateColorCalculator calculator;
    rgb_color newColor = calculator.calcPlantStateColor(measure.temperature, measure.humidity, measure.moisture);
    fader f(redPin, greenPin, bluePin);
    f.fade(currentColor, newColor);
    currentColor = newColor;

    // watering related part

    if (measure.moisture > maxMoisture) {
        watering.waterTime = true;
        watering.start = getCurrentTime();
    }
    if (watering.isNow && hasTimePassed(watering.start, wateringUnit)) {
        putWaterAway();
    }
    if (watering.waterTime && hasTimePassed(watering.start, wateringTimeout)) {
        watering.start = getCurrentTime();
        if (!measure.night) {
            waterPlant();
        }
        if (measure.moisture < minMoisture) {
            watering.waterTime = false;
        }
    }

    delay(timeout);
}

void waterPlant() {
    digitalWrite(pumpPin, 1);
    watering.isNow = true;
}

void putWaterAway() {
    digitalWrite(pumpPin, 0);
    watering.isNow = false;
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

unsigned int getCurrentTime() {
    return millis();
}

int readLight() {
    return (int) analogRead(lightPin);
}

void updateMeasurements() {
    measure.night = readLight() < nightThresholdLight;
    measure.humidity = readHumidity();
    measure.temperature = readTemperature();
    measure.moisture = readMoisture();
    measure.time = getCurrentTime();
}

bool hasTimePassed(int startTime, int interval) {
    return (getCurrentTime() - startTime) >= interval;
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

void printToLcd(Measure measurements) {
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print(measurements.time);
    lcd.print("Temperature: ");
    lcd.print(measurements.temperature);
    lcd.print("Moisture: ");
    lcd.print(measurements.moisture);
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(measurements.humidity);
    lcd.print("Night: ");
    lcd.print(measurements.night);
}
