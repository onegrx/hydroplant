//
// Created by hector on 10.01.17.
//

#include "PlantStateColorCalculator.h"
#include "../rgb_color/rgb_color.h"
#include <math.h>

rgb_color PlantStateColorCalculator::calcPlantStateColor(int t, int h, int m) {

    const int minT = 15, maxT = 25, minM = 400, maxM = 950, minH = 0, maxH = 1000; // TODO init values
    double normalizedTemperature = mapValueToNormalizedInterval(maxT, minT, t);
    double normalizedHumidity = mapValueToNormalizedInterval(maxH, minH, h);
    double normalizedMoisture = mapValueToNormalizedInterval(maxM, minM, m);

    double averageResult = normalizeInputValues(normalizedTemperature, normalizedHumidity, normalizedMoisture);

    rgb_color rgb(getRed(averageResult), getGreen(averageResult), getBlue(averageResult));

    return rgb;
}

//private

/**
 *
 * @param max worst condition
 * @param min worst condition
 * @param current condition
 * @return value mapped to [0, 1] interval where 0 is worst condition and 1 is optimal
 */
double PlantStateColorCalculator::mapValueToNormalizedInterval(int max, int min, int current) {
    double optimalValue = (max + min) / 2;
    double badToOptimal = (max - min) / 2;
    double distFromOptimal = fabs(optimalValue - current);

    return distFromOptimal >= badToOptimal ? 0 : (1 - distFromOptimal) / badToOptimal;
}

/**
 *
 * @param temperature normalized in [0, 1] interval
 * @param humidity normalized in [0, 1] interval
 * @param moisture normalized in [0, 1] interval
 * @return all params average normalized on [0, 1] interval
 */
double PlantStateColorCalculator::normalizeInputValues(double temperature, double humidity, double moisture) {
    const int temperatureWeight = 2;
    const int humidityWeight = 2;
    const int moistureWeight = 6;

    // weighted mean
    return (temperature * temperatureWeight + moisture * moistureWeight + humidity * humidityWeight)
           / (temperatureWeight + moistureWeight + humidityWeight);
}

int PlantStateColorCalculator::getBlue(double percent) {
    return 0;
}

int PlantStateColorCalculator::getRed(double percent) {
    // mapping to [1, 20] fit function range
    double percentInFitRange = percent * 19 + 1;
    double approximatedValueInFitRange = -0.000188196 * pow(percentInFitRange, 6)
                                         + 0.0117451 * pow(percentInFitRange, 5)
                                         - 0.269611 * pow(percentInFitRange, 4)
                                         + 2.75132 * pow(percentInFitRange, 3)
                                         - 12.8295 * pow(percentInFitRange, 2)
                                         + 24.3844 * percentInFitRange
                                         + 230.282;
    // mapping to [0, 255] color range
    double approximatedValueInColorRange = (approximatedValueInFitRange - 1) * 256 / 19;

    return (int) round(approximatedValueInColorRange);
}

/**
 *
 * @param percent from range [0, 1]
 * @return green rgb value from range [0, 255]
 */
int PlantStateColorCalculator::getGreen(double percent) {
    // mapping to [1, 20] fit function range
    double percentInFitRange = percent * 19 + 1;
    double approximatedValueInFitRange = -0.00019766 * pow(percentInFitRange, 6)
                                         + 0.0122973 * pow(percentInFitRange, 5)
                                         - 0.281866 * pow(percentInFitRange, 4)
                                         + 2.88042 * pow(percentInFitRange, 3)
                                         - 13.5398 * pow(percentInFitRange, 2)
                                         + 51.8622 * percentInFitRange
                                         - 33.6111;
    // mapping to [0, 255] color range
    double approximatedValueInColorRange = (approximatedValueInFitRange - 1) * 256 / 19;

    return (int) round(approximatedValueInColorRange);
}
