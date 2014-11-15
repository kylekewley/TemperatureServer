#ifndef TEMPERATURE_SERVER_h
#define TEMPERATURE_SERVER_h

#include <vector>
#include <sqlite3.h>

#include <ClientManager.h>
#include <CustomBufferParser.imp.h>

#include "SensorBridge.h"
#include "TemperatureData.pb.h"

// Server parser ids
typedef enum ServerParserID {
    TEMPERATURE_META_PARSER_ID = 1000,
    TEMPERATURE_REQUEST_PARSER_ID = 1001
}ParserID;

typedef enum ClientParserID {
    TEMPERATURE_UPDATE_PARSER_ID = 9999,
}ClientParserID;

typedef enum HistoryColumnName{
    ColumnUniqueID,
    ColumnSensorID,
    ColumnTemperature,
    ColumnHumidity,
    ColumnTimestamp,
}HistoryColumnName;

class TemperatureServer {
    public:
    TemperatureServer(std::vector<Sensor>& sensors, ClientManager& manager);


    const std::vector<Sensor>& getSensors();


    static int32_t timeSinceEpoch();
    TemperatureData* getTemperatureData(uint8_t sensorID, int32_t startTime, int32_t interval, int secondsBetweenReadings) const;

    private:
    static const std::string groupID;

    static const std::string HistoryDatabasePath;
    static const std::string HistoryTableName;

    static std::string getColumnName(HistoryColumnName column);



    sqlite3* database;

    int connectToDatabase(const string& databasePath, sqlite3** outDb);
    bool tableExists(sqlite3* db, const std::string& tablename, int* result);
    int createHistoryTable(sqlite3* db);
    int addTemperatureHistory(int sensorId, int temperature, int humidity);

    void blockAndUpdateSensors();

    std::vector<Sensor> sensors;
    std::vector<SensorBridge> bridges;

    ClientManager& clientManager;


    void registerParsers();
};

#endif
