#ifndef TEMPERATURE_SERVER_h
#define TEMPERATURE_SERVER_h

#include <vector>
#include <sqlite3.h>

#include <ClientManager.h>
#include <CustomBufferParser.imp.h>

#include "SensorBridge.h"
#include "DatabaseManager.h"
#include "TemperatureData.pb.h"

// Server parser ids
typedef enum ServerParserID {
    TEMPERATURE_META_PARSER_ID = 1000,
    TEMPERATURE_REQUEST_PARSER_ID = 1001
}ParserID;

typedef enum ClientParserID {
    TEMPERATURE_UPDATE_PARSER_ID = 9999,
}ClientParserID;

class TemperatureServer {
    public:
    TemperatureServer(std::vector<Sensor>& sensors, ClientManager& manager, int updateInterval);


    const std::vector<Sensor>& getSensors() const;

    const DatabaseManager& getDatabaseManager() const;

    private:
    static const std::string groupID;



    void blockAndUpdateSensors(int updateInterval);

    std::vector<Sensor> sensors;
    std::vector<SensorBridge> bridges;

    ClientManager& clientManager;

    DatabaseManager databaseManager;

    void registerParsers();
};

#endif
