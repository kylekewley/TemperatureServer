#include "TemperatureHistoryParser.h"
#include "TemperatureData.pb.h"

ProtocolBuffer* TemperatureHistoryParser::
parseBuffer(const TemperatureRequest* command, int) const {

    int32_t startTime = _parent.timeSinceEpoch(); //The current time
    int32_t interval = -60*60*24; //Negative 24 hours
    int timeBetweenReadings = 60; //Seconds

    if (command->has_secondsbetweenreadings())
        timeBetweenReadings = command->secondsbetweenreadings();

    if (command->has_starttime() && command->has_interval()) {
        startTime = command->starttime();
        interval = command->interval();

    }else if (command->has_starttime() && command->has_endtime()) {
        //startTime and endtime
        startTime = command->starttime();
        interval = command->endtime() - startTime;

    }else if (command->has_starttime()) {

        // Set the interval to the current time - startTime
        interval = startTime - command->starttime();

        //Starttime, no interval or endtime
        startTime = command->starttime();
    }else if (command->has_interval()){
        // Interval and no startTime

        // Backwards interval seconds from now
        interval = -::abs(command->interval());
    }

    cout << "Requesting history with interval: " << to_string(startTime)
        << " to " << to_string(startTime+interval) << endl;

    TemperatureData* buffer = _parent.getTemperatureData(command->sensorid(), 
            startTime, interval, timeBetweenReadings);

    return buffer;
}
