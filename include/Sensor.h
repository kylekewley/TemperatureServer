#ifndef SENSOR_h
#define SENSOR_h
#include <iostream>
#include <inttypes.h>

class Sensor {
    public:
    Sensor(uint8_t sensorId, uint8_t wiringPiPin, std::string sensorName):
        sensorId(sensorId), sensorName(sensorName), wiringPiPin(wiringPiPin) {};

    uint8_t sensorId;
    std::string sensorName;

    uint8_t getPin() { return wiringPiPin; };

    private:
    uint8_t wiringPiPin;
};

#endif

