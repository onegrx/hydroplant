//
// Created by hector on 10.01.17.
//

#include "PlantStateColorCalculator.h"
#include "../rgb_color/rgb_color.h"
#include <cmath>

rgb_color PlantStateColorCalculator::calcPlantStateColor(int t, int h, int m) {

    int minT, maxT, minM, maxM, minH, maxH; // TODO init values
    double normalizedTemperature = mapValueToNormalizedInterval(maxT, minT, t);
    double normalizedHumidity = mapValueToNormalizedInterval(maxH, minH, h);
    double normalizedMoisture = mapValueToNormalizedInterval(maxM, minM, m);

    double averageResult = normalizeInputValues(normalizedTemperature, normalizedHumidity, normalizedMoisture);

    rgb_color rgb(getRed(averageResult), getGreen(averageResult), getBlue(averageResult));

    return rgb;
}

//private
double PlantStateColorCalculator::mapValueToNormalizedInterval(int max, int min, int current) {
    double optimalValue = (max + min) / 2;
    double badToOptimal = (max - min) / 2;
    double distFromOptimal = std::abs(optimalValue - current);

    return distFromOptimal >= badToOptimal ? 0 : (1 - distFromOptimal) / badToOptimal;
}

double PlantStateColorCalculator::normalizeInputValues(double temperature, double humidity, double moisture) {
    int temperatureWeight = 2;
    int humidityWeight = 2;
    int moistureWeight = 6;

    return (temperature * temperatureWeight + moisture * moistureWeight + humidity * humidityWeight)
           / (temperatureWeight + moistureWeight + humidityWeight);
}

int PlantStateColorCalculator::getBlue(double percent) {
    return 0;
}

int PlantStateColorCalculator::getRed(double percent) { // TODO
    return 0;
}

int PlantStateColorCalculator::getGreen(double percent) {   // TODO
    return 0;
}
