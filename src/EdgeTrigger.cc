#include <wiringPi.h>
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <sys/time.h>

int readTemp(const int sensor, float *deg, float *hum);
int blockUntilSignal(int pin, int sig, long timeout);
void myInterrupt();

int globCount = 0;
uint64_t currentNS[45];
struct timespec currentTime;
struct timeval tv;

const int SIG_PIN = 7;
const int BCM_PIN = 4;

int iterations = 100;

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        iterations = atoi(argv[1]);
    }

    if (wiringPiSetup() < 0) {
        fprintf(stderr, "Unable to setup wiringpi: %s", strerror(errno));
        return 1;
    }

    bcm2835_init();

    if (wiringPiISR (SIG_PIN, INT_EDGE_FALLING, &myInterrupt) < 0) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror(errno)) ;
    }
    //bcm2835_gpio_afen(BCM_PIN);

    float temp;
    float hum;

    int totalIncorrect = 0;

    for (int i = 0; i < iterations; ++i) {
        totalIncorrect += readTemp(SIG_PIN, &temp, &hum);

        if (i+1 != iterations)
            delay(3000); // 3 seconds
    }

    printf("After %d iterations, %d were incorrect. %.0f%% correct\n", iterations, totalIncorrect, ((float)(iterations-totalIncorrect))/iterations*100.0);

    printf("Last temp: %.2f humidity: %.2f%%\n", temp, hum);
    return 0;
}

void myInterrupt() {
    //gettimeofday(&tv, NULL);
    //currentNS[globCount++] = tv.tv_usec;
    //
    //clock_gettime(CLOCK_REALTIME, &currentTime);
    //currentNS[globCount++] = currentTime.tv_nsec;

    //printf("Read\n");
    currentNS[globCount++] = bcm2835_st_read();
    //printf("Read result: %ld\n", currentNS[globCount-1]);
}

int blockUntilSignal(const int pin, int sig, long timeout) {
    struct timespec startTime, currentTime;
    long timeDifference = 0;

    clock_gettime(CLOCK_REALTIME, &startTime);
    do
    {
        clock_gettime(CLOCK_REALTIME, &currentTime);
        timeDifference = currentTime.tv_nsec - startTime.tv_nsec;
        if (timeDifference < 0) //Seconds rolled over
            timeDifference += 1000000000; //add one second
        if (timeDifference > timeout) //250000ns = 250 us
            return 1;
    } while(digitalRead(pin) != sig);

    return 0;
}

int readTemp(const int sensor, float *deg, float *hum) {
    piHiPri(99);

    globCount = 0;
    // Send the start signal
    pinMode(sensor, INPUT);
    if (blockUntilSignal(sensor, HIGH, 250000)) {
        printf("Error 1\n");
    }

    pinMode(sensor, OUTPUT);
    digitalWrite(sensor, LOW);
    delayMicroseconds(18000); //delay 1.1ms

    // Wait for response
    pinMode(sensor, INPUT);

    int timeoutms = 7;
    delay(timeoutms);

    int data[5] = {0, 0, 0, 0, 0};

    int j = 0;
    for (int i = 2; i < 42; ++i) {
        //printf("Time %d: %ld\n", i, (currentNS[i+1]-currentNS[i]));
        //long t = (currentNS[i+1] - currentNS[i])/1000;
        long t = (currentNS[i+1] - currentNS[i]);
        if (t < 0) t += 1000000; // 1 second

        data[j/8] <<= 1;

        if (t > 110)
            data[j/8] |= 1;

        //printf("%ld \t= %ld\n", t, data[j/8]&1);
        ++j;
    }

    float hum_p = 0.0;
    float temp_p = 0.0;

    hum_p = data[0] * 256 + data[1];
    hum_p /= 10;

    temp_p = (data[2] & 0x7F)* 256 + data[3];
    temp_p /= 10.0;
    if (data[2] & 0x80)
        temp_p *= -1;

    *hum = hum_p;
    *deg = temp_p;

    int checksum = data[4];

    return checksum != ((data[0]+data[1]+data[2]+data[3])&0xFF);
}
