#include <getopt.h>
#include <sstream>
#include <thread>
#include <chrono>

#include <sqlite3.h>

#include <wiringPi.h>

#include <PiServer.h>
#include <PiMessage.h>

#include "TemperatureServer.h"
#include "Sensor.h"
#include "SensorBridge.h"

//Parsers
#include "TemperatureMetaParser.h"
#include "TemperatureHistoryParser.h"

#define DEFAULT_PORT_NUMBER     10003

static struct option longOptions[] = {
    {"port",    required_argument,  0,  'p'}
};

typedef enum SensorID {
    OutsideSensorID,
    InsideSensorID
}SensorID;

const std::string TemperatureServer::groupID = "TemperatureGroup";
const std::string TemperatureServer::HistoryDatabasePath = "./temp_db.sqlite";
const std::string TemperatureServer::HistoryTableName = "temp_history";

int updateInterval = 60;

int main(int argc, char **argv) {
    int portNumber = DEFAULT_PORT_NUMBER;


    while (1) {
        int c = getopt_long(argc, argv, "ip:", longOptions, NULL);
        if (c == -1)
            break;
        stringstream arg;
        if (optarg)
            arg << optarg;
        switch (c) {
            case 'p':
                int portInput;
                arg >> portInput;
                if (portInput < 1024 || portInput > INT16_MAX) {
                    //Ports lower than 1024 are invalid
                    cerr << "You entered an invalid port. Connecting to default port " << to_string(portNumber) << endl;
                }else {
                    portNumber = portInput;
                }
                break;
            case 'i':
                arg >> updateInterval;
            case '?':
                break;
            default:
                cout << "?? getopt returned character code 0" << oct << c << endl;
        }
    }

    PiServer piServer = PiServer();

    //Register parsers

    vector<Sensor> sensors;

    Sensor outsideSensor(OutsideSensorID, 7, "Outside");

    sensors.push_back(outsideSensor);


    thread *t1 = new thread([&piServer, portNumber]() mutable {piServer.connectToPort(portNumber); });
    TemperatureServer(sensors, piServer.getClientManager());

    delete t1;

    return 0;
}

const std::vector<Sensor>& TemperatureServer::getSensors() {
    return sensors;
}

TemperatureServer::TemperatureServer(std::vector<Sensor>& sensors, ClientManager& manager): sensors(sensors), clientManager(manager) {
    // Load instance variables
    for (Sensor s : sensors) {
        SensorBridge bridge(s);
        if (bridge.initializeBridge()) {
            std::cerr << "Error initializing sensor with name " << s.sensorName << std::endl;
        }
        bridges.push_back(bridge);
    }

    // Register parsers
    registerParsers();

    // Open database connection
    if (connectToDatabase(HistoryDatabasePath, &database) != SQLITE_OK) {
        std::cerr << "Couldn't connect to database: temperatures will not be updated" << std::endl;
    }

    // Block the thread and update data
    blockAndUpdateSensors();

}

void TemperatureServer::blockAndUpdateSensors() {
    const int dups = 3;
    int UpdateInterval = updateInterval; // 60 seconds between updates
    int tmpCutoff = 10; // 1 degree
    int humCutoff = 100; // 10%

    int16_t temps[sensors.size()][dups];
    int16_t hums[sensors.size()][dups];

    while (true) {
        for (int i = 0; i < (int)bridges.size(); ++i) {
            SensorBridge& bridge = bridges[i];
            for (int j = 0; j < dups; ++j) {
                // Get 3 data points
                int result = bridge.dataForSensor(temps[i][j], hums[i][j]);

                // Need to redo the reading
                if (result) {
                    delay(100); //.1 second
                    --j;
                }else {
                    delay(2000); // 2 seconds
                }
            }

            // See if we got good data
            int avgTmp = 0;
            int avgHum = 0;

            for (int j = 0; j < dups; ++j) {
                avgTmp += temps[i][j];
                avgHum += hums[i][j];
            }

            avgTmp /= dups;
            avgHum /= dups;

            bool goodReadings = true;
            for (int j = 0; j < dups; ++j) {
                if (::abs(avgTmp - temps[i][j]) > tmpCutoff) {
                    goodReadings = false;
                    break;
                }

                if (::abs(avgHum - hums[i][j]) > humCutoff) {
                    goodReadings = false;
                    break;;
                }
            }

            if (!goodReadings) {
                --i;
                continue;
            }

            // Data was good
            addTemperatureHistory(sensors[i].sensorId, avgTmp, avgHum);

            std::cout << "Measured temps: ";
            for (int j = 0; j < dups; ++j) {
                std::cout << std::to_string(temps[i][j]) << ", ";
            }
            std::cout << std::endl;
            std::cout << "Adding to db. Temperature: " << std::to_string(avgTmp)
                << " Humidity: " << std::to_string(avgHum) << std::endl;
            TemperatureData d;
            d.set_sensorid(sensors[i].sensorId);

            TemperatureData_Temperature *tmp = d.add_temperatures();
            tmp->set_timestamp(timeSinceEpoch());
            tmp->set_temperature(avgTmp);
            tmp->set_humidity(avgHum);

            PiMessage message(TEMPERATURE_UPDATE_PARSER_ID, d);
            clientManager.sendMessageToGroup(message, groupID);

            delay(UpdateInterval*1000);
        }
    }
}

void TemperatureServer::registerParsers() {
    cout << "Registering parsers for Temperature Server." << endl;

    bool registerSuccess = false;

    //Register the metaParser
    shared_ptr<CustomParser> metaParser(new TemperatureMetaParser(*this));
    registerSuccess = clientManager._defaultParser->registerParserForID(
            metaParser, TEMPERATURE_META_PARSER_ID, TEMPERATURE_META_PARSER_ID);
    //Print success or failure
    cout << "Registration for GarageMetaParser ID: " <<
        to_string(TEMPERATURE_META_PARSER_ID) << " returned: " <<
        (registerSuccess ? "Success" : "Failue") << endl;

    //Register the statusParser
    shared_ptr<CustomParser> statusParser(new TemperatureHistoryParser(*this));
    registerSuccess = clientManager._defaultParser->registerParserForID(
            statusParser, TEMPERATURE_REQUEST_PARSER_ID, TEMPERATURE_REQUEST_PARSER_ID);
    //Print success or failure
    cout << "Registration for GarageStatusParser ID: " <<
        to_string(TEMPERATURE_REQUEST_PARSER_ID) << " returned: " <<
        (registerSuccess ? "Success" : "Failue") << endl;
}

// Database Methods //

int TemperatureServer::connectToDatabase(const string& databasePath, sqlite3** outDb) {
    int result = sqlite3_open_v2(databasePath.c_str(), outDb,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (result != SQLITE_OK)
        return result;

    //Create the table if it doesn't exist
    if (!tableExists(*outDb, HistoryTableName, &result)) {
        if (result != SQLITE_OK) {
            return result;
        }
        //Create the table
        result = createHistoryTable(*outDb);
        if (result != SQLITE_OK)
            return result;
    }


    return SQLITE_OK;
}

bool TemperatureServer::tableExists(sqlite3* db, const string& tableName, int* result) {

    string query = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?;";

    sqlite3_stmt* preparedStatement;
    *result = sqlite3_prepare_v2(db, query.c_str(), query.length(), 
            &preparedStatement, NULL);
    if (*result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return false;
    }

    *result = sqlite3_bind_text(preparedStatement, 1, tableName.c_str(), tableName.length(), SQLITE_STATIC);
    if (*result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return false;
    }

    *result = sqlite3_step(preparedStatement);
    if (*result >= SQLITE_ROW) {
        *result = SQLITE_OK;
    }
    if (*result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return false;
    }
    if (sqlite3_column_count(preparedStatement) == 0) {
        sqlite3_finalize(preparedStatement);
        return false;
    }

    int num = sqlite3_column_int(preparedStatement, 0);

    //Clean up
    sqlite3_finalize(preparedStatement);
    return num > 0;
}

int TemperatureServer::createHistoryTable(sqlite3* db) {
    stringstream query;
    query << "CREATE TABLE IF NOT EXISTS " << HistoryTableName << " ("
        << getColumnName(ColumnUniqueID) << " INTEGER PRIMARY KEY, "
        << getColumnName(ColumnSensorID) << " INTEGER DEFAULT (-1), "
        << getColumnName(ColumnTemperature) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnHumidity) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnTimestamp) << " INTEGER DEFAULT(strftime('%s', 'now')));" 
        << endl;

    sqlite3_stmt* preparedStatement;
    int result = sqlite3_prepare_v2(db, query.str().c_str(), query.str().length(), &preparedStatement, NULL);
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return result;
    }

    result = sqlite3_step(preparedStatement);
    if (result >= SQLITE_ROW) {
        result = SQLITE_OK;
    }
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return result;
    }

    sqlite3_finalize(preparedStatement);
    return result;
}

// Querying the Database //

TemperatureData* TemperatureServer::getTemperatureData(uint8_t sensorID, int32_t startTime, int32_t interval, int secondsBetweenReadings) const {
    stringstream query;
    query << "SELECT * FROM " << HistoryTableName << " "
        << "WHERE " << getColumnName(ColumnTimestamp) << " BETWEEN ? AND ? AND"
        << getColumnName(ColumnSensorID) << " == ? ORDER BY "
        << getColumnName(ColumnTimestamp) << " ASC;";

    sqlite3_stmt* preparedStatement;
    int result = sqlite3_prepare_v2(database, query.str().c_str(), query.str().length(), &preparedStatement, NULL);
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return NULL;
    }

    result = sqlite3_bind_int(preparedStatement, 1, startTime);
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return NULL;
    }

    result = sqlite3_bind_int(preparedStatement, 2, startTime+interval);
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return NULL;
    }

    result = sqlite3_bind_int(preparedStatement, 3, sensorID);
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return NULL;
    }

    TemperatureData* tempData = new TemperatureData();
    tempData->set_sensorid(sensorID);

    int32_t lastReadingTime = 0;

    while (sqlite3_step(preparedStatement) == SQLITE_ROW) {
        int64_t rowId = sqlite3_column_int64(preparedStatement, ColumnUniqueID);
        int32_t temperature = sqlite3_column_int(preparedStatement, ColumnTemperature);
        int32_t humidity = sqlite3_column_int(preparedStatement, ColumnHumidity);
        int32_t timestamp = sqlite3_column_int(preparedStatement, ColumnTimestamp);

        if (::abs(timestamp - lastReadingTime) < secondsBetweenReadings) {
            continue;
        }

        TemperatureData_Temperature* temp = tempData->add_temperatures();
        temp->set_timestamp(timestamp);
        temp->set_uniqueid((uint32_t)rowId);
        temp->set_humidity(humidity);
        temp->set_temperature(temperature);
    }
    sqlite3_finalize(preparedStatement);

    return tempData;
}

int TemperatureServer::addTemperatureHistory(int sensorId, int temperature, int humidity) {
    stringstream query;
    query << "INSERT INTO " << HistoryTableName << " ("
        << getColumnName(ColumnSensorID) << ", "
        << getColumnName(ColumnTemperature) << ", "
        << getColumnName(ColumnHumidity) << ") "
        << "VALUES ("
        << to_string(sensorId) << ", "
        << to_string(temperature) << ", "
        << to_string(humidity) << ");";

    sqlite3_stmt* preparedStatement;
    int result = sqlite3_prepare_v2(database, query.str().c_str(), query.str().length(), &preparedStatement, NULL);
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return result;
    }

    result = sqlite3_step(preparedStatement);
    if (result >= SQLITE_ROW) {
        result = SQLITE_OK;
    }
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return result;
    }

    sqlite3_finalize(preparedStatement);
    return result;
}

// Static Methods //

int32_t TemperatureServer::timeSinceEpoch() {
    return std::chrono::duration_cast<std::chrono::seconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string TemperatureServer::getColumnName(HistoryColumnName column) {
    switch(column) {
        case ColumnUniqueID:
            return "unique_id";
        case ColumnSensorID:
            return "sensor_id";
        case ColumnTemperature:
            return "temperature";
        case ColumnHumidity:
            return "humidity";
        case ColumnTimestamp:
            return "timestamp";
    }

    return "";
}
