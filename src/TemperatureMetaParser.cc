#include "TemperatureMetaParser.h"

#include "Sensor.h"


/* ##Parser Subclasses## */
/* --------------------- */
ProtocolBuffer* TemperatureMetaParser::
parseBuffer(const ServerMetaData*, int) const {
    ServerMetaData* metaData = new ServerMetaData();

    const vector<Sensor>& sensors = _parent.getSensors();

    for (const Sensor& sensor : sensors) {
        ServerMetaData_Sensor* addSensor = metaData->add_sensors();

        addSensor->set_sensorid(sensor.sensorId);
        addSensor->set_sensorname(sensor.sensorName);

    }

    return metaData;
}
