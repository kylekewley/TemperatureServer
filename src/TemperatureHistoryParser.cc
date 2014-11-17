#include "TemperatureHistoryParser.h"
#include "TemperatureData.pb.h"

ProtocolBuffer* TemperatureHistoryParser::
parseBuffer(const TemperatureRequest* command, int) const {

    int32_t startTime = DatabaseManager::localDayStartEpoch();
    int32_t endTime = DatabaseManager::timeSinceEpoch();

    int timeBetweenReadings = 0; //Seconds

    bool summaryOnly = false;

    if (command->has_secondsbetweenreadings())
        timeBetweenReadings = command->secondsbetweenreadings();

    if (command->has_summaryonly())
        summaryOnly = command->summaryonly();


    if (command->has_starttime()) {
        startTime = command->starttime();

        if (command->has_endtime()) {
            // Start and end
            endTime = command->endtime();
        }else if (command->has_interval()) {
            // Start and interval
            endTime = startTime + command->interval();
        }
    }else if (command->has_interval()) {
        // Just an interval time
        startTime = DatabaseManager::timeSinceEpoch()-::abs(command->interval());
    }

    cout << "Requesting history with interval: " << to_string(startTime)
        << " to " << to_string(endTime) << endl;

    TemperatureData* buffer = _parent.getDatabaseManager().getTemperatureData(command->sensorid(),
            startTime, endTime, timeBetweenReadings, summaryOnly);

    return buffer;
}
