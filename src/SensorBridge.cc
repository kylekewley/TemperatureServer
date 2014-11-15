#include <wiringPi.h>
#include <bcm2835.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#include "SensorBridge.h"

int globCount = 0;
uint64_t currentNS[45];
void fallingEdgeInturrupt();

bool SensorBridge::bridgeBusy = false;

int SensorBridge::initializeBridge() {
    if (wiringPiSetup() < 0) {
        fprintf(stderr, "Unable to setup wiringpi: %s", strerror(errno));
        return 1;
    }

    bcm2835_init();

    if (wiringPiISR(sensor.getPin(), INT_EDGE_FALLING, &fallingEdgeInturrupt) < 0) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror(errno)) ;
    }

    initialized = true;
    return 0;
}

void fallingEdgeInturrupt() {
    currentNS[globCount++] = bcm2835_st_read();
}


int SensorBridge::blockUntilSignal(const int pin, const int sig, const long usTimeout) {
    long timeDifference = 0;
    uint64_t startus = bcm2835_st_read();
    uint64_t currentus;

    do {
        currentus = bcm2835_st_read();
        timeDifference = currentus - startus;

        if (timeDifference < 0) //Seconds rolled over
            timeDifference += 1000000; //add one second

        if (timeDifference > usTimeout)
            return 1;

    } while(digitalRead(pin) != sig);

    return 0;
}

int SensorBridge::dataForSensor(int16_t& temp, int16_t& hum) {
    if (!initialized) {
        return 2;
    }
    if (bridgeBusy) {
        return 3;
    }

    bridgeBusy = true;

    piHiPri(99);

    uint8_t pin = sensor.getPin();
    globCount = 0;
    // Wait to pull up to high
    pinMode(pin, INPUT);

    if (blockUntilSignal(pin, HIGH, 250000)) {
        // Error. Didn't recieve correct signal
        bridgeBusy = false;
        return 1;
    }

    // Send start signal for 18ms
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(18000);

    // Switch back to input. Our falling edge inturrupt collects data
    pinMode(pin, INPUT);

    // The data transfer should take no longer than 7ms
    int timeoutms = 7;
    delay(timeoutms);


    // Convert the data stored in currentNS
    int data[5] = {0, 0, 0, 0, 0};

    int j = 0;
    const int cutoff = 110;
    for (int i = 2; i < 42; ++i) {
        long t = (currentNS[i+1] - currentNS[i]);
        if (t < 0) t += 1000000; // 1 second

        data[j/8] <<= 1;

        if (t > cutoff)
            data[j/8] |= 1;

        ++j;
    }

    bridgeBusy = false;

    hum = (data[0]<<8) + data[1];

    temp = ((data[2] & 0x7F)<<8) + data[3];
    if (data[2] & 0x80)
        temp *= -1;

    int checksum = data[4];

    return checksum != ((data[0]+data[1]+data[2]+data[3])&0xFF);
}
