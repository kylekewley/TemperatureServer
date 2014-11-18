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
#define DEFAULT_UPDATE_INTERVAL 60

const static int MaxDegreeVariation = 1; // Degree C
const static int MaxHumidityVariation = 10; // % Humidity
const static int DuplicateReadings = 3; // Number of readings for the average


static struct option longOptions[] = {
    {"port",    required_argument,  0,  'p'}
};

typedef enum SensorID {
    OutsideSensorID,
    InsideSensorID
}SensorID;
const std::string TemperatureServer::groupID = "TemperatureGroup";

int main(int argc, char **argv) {
    int portNumber = DEFAULT_PORT_NUMBER;
    int updateInterval = DEFAULT_UPDATE_INTERVAL;

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
    TemperatureServer(sensors, piServer.getClientManager(), updateInterval);

    delete t1;

    return 0;
}

const std::vector<Sensor>& TemperatureServer::getSensors() const {
    return sensors;
}

const DatabaseManager& TemperatureServer::getDatabaseManager() const {
    return databaseManager;
}

TemperatureServer::TemperatureServer(std::vector<Sensor>& sensors, ClientManager& manager, int updateInterval): sensors(sensors), clientManager(manager) {
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
    if (databaseManager.connectToDatabase() != SQLITE_OK) {
        std::cerr << "Couldn't connect to database: temperatures will not be updated" << std::endl;
    }
    databaseManager.initializeSensorData(sensors);

    // Block the thread and update data
    blockAndUpdateSensors(updateInterval);

}

void TemperatureServer::blockAndUpdateSensors(int updateInterval) {
    const int dups = DuplicateReadings;

    // We read and store in 10ths of a degree. Multiply by 10 to get the cutoff
    int tmpCutoff = MaxDegreeVariation * 10;
    int humCutoff = MaxHumidityVariation * 10; // 10%

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
            databaseManager.updateCurrentDayRecord(sensors[i].sensorId, avgTmp, avgHum);
            databaseManager.addTemperatureHistory(sensors[i].sensorId, avgTmp, avgHum);

            TemperatureData d;
            d.set_sensorid(sensors[i].sensorId);

            TemperatureData_SingleDay *tmp = d.add_daydata();

            TemperatureData_Temperature *current = tmp->add_temperatures();

            current->set_timestamp(DatabaseManager::timeSinceEpoch());
            current->set_temperature(avgTmp);
            current->set_humidity(avgHum);

            PiMessage message(TEMPERATURE_UPDATE_PARSER_ID, d);
            clientManager.sendMessageToGroup(message, groupID);

            delay(updateInterval*1000);
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


