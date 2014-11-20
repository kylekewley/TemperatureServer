#include "TemperatureMetaParser.h"

#include "Sensor.h"

/* ##Parser Subclasses## */
/* --------------------- */
ProtocolBuffer* TemperatureMetaParser::
parseBuffer(const ServerMetaData*, int) const {
    ServerMetaData* metaData = new ServerMetaData();

    const vector<Sensor>& sensors = _parent.getSensors();

    metaData->set_timezoneoffset(DatabaseManager::getTimezoneOffset());
    metaData->set_longitude(TemperatureServer::getLongitude());
    metaData->set_latitude(TemperatureServer::getLatitude());



    for (const Sensor& sensor : sensors) {
        ServerMetaData_Sensor* addSensor = metaData->add_sensors();

        addSensor->set_sensorid(sensor.sensorId);
        addSensor->set_sensorname(sensor.sensorName);

    }

    return metaData;
}
