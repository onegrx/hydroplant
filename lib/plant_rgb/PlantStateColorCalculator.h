//
// Created by hector on 10.01.17.
//

#ifndef ARDUINO_PLANT_STATE_COLOR_CALCULATOR_H
#define ARDUINO_PLANT_STATE_COLOR_CALCULATOR_H

#include "../rgb_color/rgb_color.h"

struct Color {
    int red;
    int green;
    int blue;
};

class PlantStateColorCalculator {
private:
    double mapValueToNormalizedInterval(int max, int min, int current);
    double normalizeInputValues(double temperature, double humidity, double moisture);
    int getGreen(double percent);
    int getRed(double percent);
    int getBlue(double percent);
public:
    rgb_color calcPlantStateColor(int t, int h, int m);
};


#endif //ARDUINO_PLANT_STATE_COLOR_CALCULATOR_H
