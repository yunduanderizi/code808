#ifndef _BUS_STATION_TABLE_H_
#define _BUS_STATION_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    BUS_STATION_NUMBER_ID = 0,
    BUS_STATION_NAME,
    BUS_STATION_LOC_FLAG,
    BUS_STATION_LATITUDE,
    BUS_STATION_LONGITUDE,
    BUS_STATION_DISTANCE,
    BUS_STATION_COLNUM_BUTT
}ENUM_BUS_STATION_COL;

class BusStationUpTable : public MultiRowWithUniqueIndexTable
{
public:
    BusStationUpTable();
};

class BusStationDownTable : public MultiRowWithUniqueIndexTable
{
public:
    BusStationDownTable();
};

#endif
