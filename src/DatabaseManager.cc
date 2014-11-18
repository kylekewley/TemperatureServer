#include "DatabaseManager.h"

#include <sstream>
#include <chrono>
#include <time.h>

#include <sqlite3.h>

const std::string DatabaseManager::HistoryDatabasePath = "./temp_db.sqlite";
const std::string DatabaseManager::HistoryTableName = "temp_history";
const std::string DatabaseManager::SummaryTableName = "temp_summary";

const long DatabaseManager::TimezoneOffset = getTimezoneOffset();

// Database Methods //

int DatabaseManager::connectToDatabase() {
    return connectToDatabase(HistoryDatabasePath, &database);
}

int DatabaseManager::connectToDatabase(const std::string& databasePath, sqlite3** outDb) {
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

    if (!tableExists(*outDb, SummaryTableName, &result)) {
        if (result != SQLITE_OK) {
            return result;
        }
        //Create the table
        result = createSummaryTable(*outDb);
        if (result != SQLITE_OK)
            return result;
    }

    return SQLITE_OK;
}

bool DatabaseManager::tableExists(sqlite3* db, const std::string& tableName, int* result) {

    std::string query = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?;";

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

 void DatabaseManager::initializeSensorData(std::vector<Sensor> sensors) {
     for (Sensor s : sensors) {
         std::stringstream query;
         // Get the day summaries
         query << "SELECT * FROM " << SummaryTableName << " "
             << "WHERE " << getColumnName(ColumnTimestamp) << " == ? AND "
             << getColumnName(ColumnSensorID) << " == ?;";

         sqlite3_stmt* summaryStatement;

         int result = sqlite3_prepare_v2(database, query.str().c_str(), query.str().length(), &summaryStatement, NULL);
         if (result != SQLITE_OK) {
             sqlite3_finalize(summaryStatement);
             return;
         }

         result = sqlite3_bind_int(summaryStatement, 1, localDayStartEpoch());
         if (result != SQLITE_OK) {
             sqlite3_finalize(summaryStatement);
             return;
         }

         result = sqlite3_bind_int(summaryStatement, 2, s.sensorId);
         if (result != SQLITE_OK) {
             sqlite3_finalize(summaryStatement);
             return;
         }

         // Query for detailed data
         query.str("");
         query << "SELECT * FROM " << HistoryTableName << " "
             << "WHERE " << getColumnName(ColumnDayForeignKey) << " == ? ORDER BY "
             << getColumnName(ColumnTimestamp) << " ASC;" << std::endl;

         sqlite3_stmt* preparedStatement;
         result = sqlite3_prepare_v2(database, query.str().c_str(), query.str().length(), &preparedStatement, NULL);
         if (result != SQLITE_OK) {
             sqlite3_finalize(preparedStatement);
             return;
         }

         int i = 0;
         while (sqlite3_step(summaryStatement) == SQLITE_ROW) {
             ++i;
             int64_t rowId = sqlite3_column_int64(summaryStatement, ColumnUniqueID);
             int32_t currentTemp = sqlite3_column_int(summaryStatement, ColumnCurrentTemperature);
             int32_t maxTemp = sqlite3_column_int(summaryStatement, ColumnMaxTemperature);
             int32_t minTemp = sqlite3_column_int(summaryStatement, ColumnMinTemperature);
             int32_t maxTempTime = sqlite3_column_int(summaryStatement, ColumnMaxTemperatureTime);
             int32_t minTempTime = sqlite3_column_int(summaryStatement, ColumnMinTemperatureTime);
             int32_t currentTempTime = sqlite3_column_int(summaryStatement, ColumnCurrentTemperatureTime);


             TemperatureData_SingleDay day;

             TemperatureData_Temperature *temp = new TemperatureData_Temperature();
             temp->set_timestamp(currentTempTime);
             temp->set_temperature(currentTemp);
             day.set_allocated_currenttemperature(temp);

             temp = new TemperatureData_Temperature();
             temp->set_timestamp(maxTempTime);
             temp->set_temperature(maxTemp);
             day.set_allocated_dayhigh(temp);

             temp = new TemperatureData_Temperature();
             temp->set_timestamp(minTempTime);
             temp->set_temperature(minTemp);
             day.set_allocated_daylow(temp);

             currentDayRecordEpoch[s.sensorId] = localDayStartEpoch();
             currentDayRecordID[s.sensorId] = rowId;


             // Detailed data too
             result = sqlite3_bind_int(preparedStatement, 1, s.sensorId);
             if (result != SQLITE_OK) {
                 sqlite3_finalize(preparedStatement);
                 return;
             }

             while (sqlite3_step(preparedStatement) == SQLITE_ROW) {
                 int64_t rowId = sqlite3_column_int64(preparedStatement, ColumnUniqueID);
                 int32_t temperature = sqlite3_column_int(preparedStatement, ColumnTemperature);
                 int32_t humidity = sqlite3_column_int(preparedStatement, ColumnHumidity);
                 int32_t timestamp = sqlite3_column_int(preparedStatement, ColumnTimestamp);

                 TemperatureData_Temperature* temp = day.add_temperatures();
                 temp->set_timestamp(timestamp);
                 temp->set_uniqueid((uint32_t)rowId);
                 temp->set_humidity(humidity);
                 temp->set_temperature(temperature);
            }

             currentDayData[s.sensorId] = day;
         }
         if (i == 0) {
             // There was no data
         }

     }
}
int DatabaseManager::createSummaryTable(sqlite3* db) {
    std::stringstream query;
    sqlite3_stmt* preparedStatement;

    query << "CREATE TABLE IF NOT EXISTS " << SummaryTableName << " ("
        << getColumnName(ColumnUniqueSummaryID) << " INTEGER PRIMARY KEY, "
        << getColumnName(ColumnSensorSummaryID) << " INTEGER DEFAULT (-1), "
        << getColumnName(ColumnSummaryTimestamp) << " INTEGER, "
        << getColumnName(ColumnCurrentTemperature) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnMaxTemperature) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnMinTemperature) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnCurrentTemperatureTime) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnMaxTemperatureTime) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnMinTemperatureTime) << " INTEGER DEFAULT (0));"
        << std::endl;

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

int DatabaseManager::createHistoryTable(sqlite3* db) {
    std::stringstream query;
    query << "CREATE TABLE IF NOT EXISTS " << HistoryTableName << " ("
        << getColumnName(ColumnUniqueID) << " INTEGER PRIMARY KEY, "
        << getColumnName(ColumnSensorID) << " INTEGER DEFAULT (-1), "
        << getColumnName(ColumnTimestamp) << " INTEGER DEFAULT(strftime('%s', 'now')), " 
        << getColumnName(ColumnTemperature) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnHumidity) << " INTEGER DEFAULT (0), "
        << getColumnName(ColumnDayForeignKey) << " INTEGER DEFAULT (0));"
        << std::endl;

    std::cout << query.str() << std::endl;
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

TemperatureData *DatabaseManager::getTemperatureData(uint8_t sensorID, int32_t startTime, int32_t endTime, int secondsBetweenReadings, bool summary) const {
    std::stringstream query;

    // Make sure startTime is before endTime
    if (startTime > endTime) {
        int32_t tmp = startTime;
        startTime = endTime;
        endTime = tmp;
    }


    // Get the day summaries
    query << "SELECT * FROM " << SummaryTableName << " "
        << "WHERE " << getColumnName(ColumnTimestamp) << " BETWEEN ? AND ? AND "
        << getColumnName(ColumnSensorID) << " == ?;";

    sqlite3_stmt* summaryStatement;

    int result = sqlite3_prepare_v2(database, query.str().c_str(), query.str().length(), &summaryStatement, NULL);
    if (result != SQLITE_OK) {
        sqlite3_finalize(summaryStatement);
        return NULL;
    }

    result = sqlite3_bind_int(summaryStatement, 1, localDayStartEpoch(startTime)-1);
    if (result != SQLITE_OK) {
        sqlite3_finalize(summaryStatement);
        return NULL;
    }

    result = sqlite3_bind_int(summaryStatement, 2, localDayStartEpoch(endTime)+1);
    if (result != SQLITE_OK) {
        sqlite3_finalize(summaryStatement);
        return NULL;
    }

    result = sqlite3_bind_int(summaryStatement, 3, sensorID);
    if (result != SQLITE_OK) {
        sqlite3_finalize(summaryStatement);
        return NULL;
    }

    std::cout << query.str() << std::endl;
    std::cout << "sID: " << std::to_string(sensorID) << std::endl;

    // Query for detailed data
    query.str("");
    query << "SELECT * FROM " << HistoryTableName << " "
        << "WHERE " << getColumnName(ColumnDayForeignKey) << " == ? ORDER BY "
        << getColumnName(ColumnTimestamp) << " ASC;" << std::endl;

    sqlite3_stmt* preparedStatement;
    result = sqlite3_prepare_v2(database, query.str().c_str(), query.str().length(), &preparedStatement, NULL);
    if (result != SQLITE_OK) {
        sqlite3_finalize(preparedStatement);
        return NULL;
    }



    TemperatureData *tempData = new TemperatureData();
    tempData->set_sensorid(sensorID);

    while (sqlite3_step(summaryStatement) == SQLITE_ROW) {
        int64_t rowId = sqlite3_column_int64(summaryStatement, ColumnUniqueID);
        //int32_t timestamp = sqlite3_column_int(summaryStatement, ColumnTimestamp);
        int32_t currentTemp = sqlite3_column_int(summaryStatement, ColumnCurrentTemperature);
        int32_t maxTemp = sqlite3_column_int(summaryStatement, ColumnMaxTemperature);
        int32_t minTemp = sqlite3_column_int(summaryStatement, ColumnMinTemperature);
        int32_t maxTempTime = sqlite3_column_int(summaryStatement, ColumnMaxTemperatureTime);
        int32_t minTempTime = sqlite3_column_int(summaryStatement, ColumnMinTemperatureTime);
        int32_t currentTempTime = sqlite3_column_int(summaryStatement, ColumnCurrentTemperatureTime);

        std::cout << "Has data " << std::to_string(currentTemp) << std::endl;

        TemperatureData_SingleDay *day = tempData->add_daydata();

        TemperatureData_Temperature *temp = new TemperatureData_Temperature();
        temp->set_timestamp(maxTempTime);
        temp->set_temperature(maxTemp);
        day->set_allocated_dayhigh(temp);

        temp = new TemperatureData_Temperature();
        temp->set_timestamp(minTempTime);
        temp->set_temperature(minTemp);
        day->set_allocated_daylow(temp);

        temp = new TemperatureData_Temperature();
        temp->set_timestamp(currentTempTime);
        temp->set_temperature(currentTemp);


        if (!summary) {
            // Detailed data too
            result = sqlite3_bind_int(preparedStatement, 1, rowId);
            if (result != SQLITE_OK) {
                sqlite3_finalize(preparedStatement);
                return NULL;
            }

            int32_t lastReadingTime = 0;

            int32_t humidity = -1;
            while (sqlite3_step(preparedStatement) == SQLITE_ROW) {
                int64_t rowId = sqlite3_column_int64(preparedStatement, ColumnUniqueID);
                int32_t temperature = sqlite3_column_int(preparedStatement, ColumnTemperature);
                humidity = sqlite3_column_int(preparedStatement, ColumnHumidity);
                int32_t timestamp = sqlite3_column_int(preparedStatement, ColumnTimestamp);

                if (::abs(timestamp - lastReadingTime) < secondsBetweenReadings) {
                    continue;
                }

                TemperatureData_Temperature* temp = day->add_temperatures();
                temp->set_timestamp(timestamp);
                temp->set_uniqueid((uint32_t)rowId);
                temp->set_humidity(humidity);
                temp->set_temperature(temperature);
            }

            if (humidity >= 0) {
                temp->set_humidity(humidity);
            }
        }

        day->set_allocated_currenttemperature(temp);
    }


    sqlite3_finalize(summaryStatement);
    sqlite3_finalize(preparedStatement);

    return tempData;
}

int DatabaseManager::updateCurrentDayRecord(int sensorid, uint32_t temperature, uint32_t humidity) {
    std::stringstream query;

    int result;
    if (localDayStartEpoch() == currentDayRecordEpoch[sensorid]) {
        // Update old entry
        int32_t currentTime = timeSinceEpoch();
        TemperatureData_SingleDay& dayData = currentDayData[sensorid];

        TemperatureData_Temperature *newCurrent = new TemperatureData_Temperature();
        newCurrent->set_timestamp(currentTime);
        newCurrent->set_temperature(temperature);
        newCurrent->set_humidity(humidity);

        dayData.set_allocated_currenttemperature(newCurrent);

        if (temperature > dayData.dayhigh().temperature()) {
            // Set the new high
            TemperatureData_Temperature *newHigh = new TemperatureData_Temperature(*newCurrent);
            dayData.set_allocated_dayhigh(newHigh);
        }

        if (temperature < dayData.daylow().temperature()) {
            // Set the new high
            TemperatureData_Temperature *newLow = new TemperatureData_Temperature(*newCurrent);
            dayData.set_allocated_daylow(newLow);
        }


        query << "UPDATE " << SummaryTableName << " SET "
            << getColumnName(ColumnCurrentTemperature) << " =  " << std::to_string(temperature) << ", "
            << getColumnName(ColumnCurrentTemperatureTime) << " =  " << std::to_string(currentTime) << ", "
            << getColumnName(ColumnMaxTemperature) << " =  " << std::to_string(dayData.dayhigh().temperature()) << ", "
            << getColumnName(ColumnMaxTemperatureTime) << " =  " << std::to_string(dayData.dayhigh().timestamp()) << ", "
            << getColumnName(ColumnMinTemperature) << " =  " << std::to_string(dayData.daylow().temperature()) << ", "
            << getColumnName(ColumnMinTemperatureTime) << " =  " << std::to_string(dayData.daylow().timestamp()) 
            << " WHERE " << getColumnName(ColumnUniqueID) << " = " << currentDayRecordID[sensorid] << ";";

        sqlite3_stmt* preparedStatement;
        result = sqlite3_prepare_v2(database, query.str().c_str(), query.str().length(), &preparedStatement, NULL);
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


    }else {
        // Create new entry
        //
        int32_t currentTime = timeSinceEpoch();
        currentDayRecordEpoch[sensorid] = localDayStartEpoch();

        TemperatureData_SingleDay dayData;

        TemperatureData_Temperature *current = new TemperatureData_Temperature();
        current->set_timestamp(currentTime);
        current->set_humidity(humidity);
        current->set_temperature(temperature);

        TemperatureData_Temperature *high = new TemperatureData_Temperature(*current);
        TemperatureData_Temperature *low = new TemperatureData_Temperature(*current);

        dayData.set_allocated_daylow(low);
        dayData.set_allocated_dayhigh(high);
        dayData.set_allocated_currenttemperature(current);

        query << "INSERT INTO " << SummaryTableName << " ("
            << getColumnName(ColumnTimestamp) << ", "
            << getColumnName(ColumnSensorID) << ", "
            << getColumnName(ColumnCurrentTemperature) << ", "
            << getColumnName(ColumnCurrentTemperatureTime) << ", "
            << getColumnName(ColumnMaxTemperature) << ", "
            << getColumnName(ColumnMaxTemperatureTime) << ", "
            << getColumnName(ColumnMinTemperature) << ", "
            << getColumnName(ColumnMinTemperatureTime) << ") "
            << "VALUES ("
            << std::to_string(localDayStartEpoch()) << ", "
            << std::to_string(sensorid) << ", "
            << std::to_string(temperature) << ", "
            << std::to_string(currentTime) << ", "
            << std::to_string(temperature) << ", "
            << std::to_string(currentTime) << ", "
            << std::to_string(temperature) << ", "
            << std::to_string(currentTime) << ");";

        std::cout << query.str() << std::endl;

        sqlite3_stmt* preparedStatement;
        result = sqlite3_prepare_v2(database, query.str().c_str(), query.str().length(), &preparedStatement, NULL);
        if (result != SQLITE_OK) {
            std::cout << "Error: " << std::to_string(result) << std::endl;
            sqlite3_finalize(preparedStatement);
            return result;
        }

        result = sqlite3_step(preparedStatement);
        if (result >= SQLITE_ROW) {
            result = SQLITE_OK;
        }

        sqlite3_finalize(preparedStatement);

        currentDayRecordID[sensorid] = sqlite3_last_insert_rowid(database);
        currentDayData[sensorid] = dayData;
    }

    return result;
}

int DatabaseManager::addTemperatureHistory(int sensorID, int temperature, int humidity) {
    std::stringstream query;
    query << "INSERT INTO " << HistoryTableName << " ("
        << getColumnName(ColumnSensorID) << ", "
        << getColumnName(ColumnTemperature) << ", "
        << getColumnName(ColumnDayForeignKey) << ", "
        << getColumnName(ColumnHumidity) << ") "
        << "VALUES ("
        << std::to_string(sensorID) << ", "
        << std::to_string(temperature) << ", "
        << std::to_string(currentDayRecordID[sensorID]) << ", "
        << std::to_string(humidity) << ");";

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

int32_t DatabaseManager::timeSinceEpoch() {
    return std::chrono::duration_cast<std::chrono::seconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}

int32_t DatabaseManager::localDayStartEpoch() {
    return localDayStartEpoch(timeSinceEpoch());
}

long DatabaseManager::getTimezoneOffset() {
    // Get the timezone offset
    time_t t = time(NULL);
    struct tm lt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    localtime_r(&t, &lt);

    return lt.tm_gmtoff;
}

int32_t DatabaseManager::localDayStartEpoch(int32_t epoch) {
    long dayInSeconds = 24*60*60;

    int32_t startDay = (epoch - (epoch % dayInSeconds)) - TimezoneOffset;

    if (startDay > epoch) {
        startDay -= dayInSeconds;
    }

    return startDay;
}

std::string DatabaseManager::getColumnName(SummaryColumnName column) {
    switch(column) {
        case ColumnUniqueSummaryID:
            return "unique_id";
        case ColumnSensorSummaryID:
            return "sensor_id";
        case ColumnSummaryTimestamp:
            return "timestamp";
        case ColumnMaxTemperature:
            return "max_temp";
        case ColumnMaxTemperatureTime:
            return "max_temp_epoch";
        case ColumnMinTemperature:
            return "min_temp";
        case ColumnMinTemperatureTime:
            return "min_temp_epoch";
        case ColumnCurrentTemperature:
            return "current_temp";
        case ColumnCurrentTemperatureTime:
            return "current_temp_time";
    }

    return "";
 }

std::string DatabaseManager::getColumnName(HistoryColumnName column) {
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
        case ColumnDayForeignKey:
            return "day_id";
   }

    return "";
}
