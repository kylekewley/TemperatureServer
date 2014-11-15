#ifndef SENSOR_BRIDGE_h
#define SENSOR_BRIDGE_h

#include <vector>

#include "Sensor.h"

class SensorBridge {

    public:
    SensorBridge(Sensor s);

    /**
     * Initializes the wiring pi library 
     * @return 0 for success. A non-zero number upon failure.
     */
    int initializeBridge();

    /**
     * Gets the data for the temperature sensor
     *
     * @param *out* temp    The temperature in C multiplied by 10
     * @param *out* hum     The percent humidity multiplied by 10
     *
     * @return 0 upon success. If not zero, the temperature and humidity
     *          could be incorrect and another request should be made
     */
    int dataForSensor(int16_t& temp, int16_t& hum);

    private:
    Sensor sensor;

    /**
     * Blocks until the timeout or the pin value == sig.
     *
     * @param   pin The pin to read.
     * @param   sig Either HIGH or LOW.
     * @param   usTimeout   The timeout for the blocking call in microseconds
     *
     * @return 0 if the pin value == sig before the timeout. 1 if we hit the timout.
     */
    static int blockUntilSignal(const int pin, const int sig, const long usTimeout);

    static bool bridgeBusy;

    bool initialized;
};

#endif

