#include "BusStationTable.h"

const static SQLiteColumn BusStationCols[BUS_STATION_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColNumber",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColName",             ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false),
    SQLiteColumn("ColLocFlag",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLatitude",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLongitude",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColDistance",         ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false)
};

BusStationUpTable::BusStationUpTable():
    MultiRowWithUniqueIndexTable(BUS_STATION_UP_DB_NAME,
                                 BUS_STATION_UP_TABLE_NAME, 
                                 (int)BUS_STATION_MAX_ROWS, 
                                 (int)BUS_STATION_COLNUM_BUTT, 
                                 BusStationCols, 
                                 (int)BUS_STATION_NUMBER_ID)
{
    ;
}

BusStationDownTable::BusStationDownTable():
    MultiRowWithUniqueIndexTable(BUS_STATION_DOWN_DB_NAME,
                                 BUS_STATION_DOWN_TABLE_NAME, 
                                 (int)BUS_STATION_MAX_ROWS, 
                                 (int)BUS_STATION_COLNUM_BUTT, 
                                 BusStationCols, 
                                 (int)BUS_STATION_NUMBER_ID)
{
    ;
}
