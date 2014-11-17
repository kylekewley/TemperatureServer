#ifndef DATABASE_MANAGER_h
#define DATABASE_MANAGER_h

#include <iostream>

#include <sqlite3.h>

#include "TemperatureData.pb.h"
typedef enum HistoryColumnName{
    /* Shared */
    ColumnUniqueID,
    ColumnSensorID,
    ColumnTimestamp,

    /* Detailed History Table */
    ColumnTemperature,
    ColumnHumidity,
    ColumnDayForeignKey,

    /* Day summary table */
    ColumnMaxTemperature,
    ColumnMaxTemperatureTime,
    ColumnMinTemperature,
    ColumnMinTemperatureTime,
    ColumnCurrentTemperature,
    ColumnCurrentTemperatureTime,

}HistoryColumnName;


class DatabaseManager {
    public:
    int connectToDatabase();

    TemperatureData* getTemperatureData(uint8_t sensorID, int32_t startTime, int32_t endTime, int secondsBetweenReadings, bool summary) const;

    int updateCurrentDayRecord(int sensorid, uint32_t temperature, uint32_t humidity);
    int addTemperatureHistory(int sensorId, int temperature, int humidity);

    static int32_t timeSinceEpoch();

    /*
     * Returns the epoch for 12:00AM on the current day
     */
    static int32_t localDayStartEpoch();

    /*
     * Returns the epoch for 12:00AM on the day specified by epoch.
     */
    static int32_t localDayStartEpoch(int32_t epoch);

    private:
    sqlite3* database;

    static const std::string HistoryDatabasePath;
    static const std::string HistoryTableName;
    static const std::string SummaryTableName;

    static const long TimezoneOffset;

    static long getTimezoneOffset();

    static std::string getColumnName(HistoryColumnName column);


    bool tableExists(sqlite3* db, const std::string& tablename, int* result);

    int connectToDatabase(const std::string& databasePath, sqlite3** outDb);
    int createHistoryTable(sqlite3* db);
    int createSummaryTable(sqlite3* db);


    std::map<uint8_t, TemperatureData_SingleDay> currentDayData;
    std::map<uint8_t, int32_t> currentDayRecordEpoch;
    std::map<uint8_t, int64_t> currentDayRecordID;
};

#endif

