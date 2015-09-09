#ifndef _VDR_BUS_RECORD_TABLE_H_
#define _VDR_BUS_RECORD_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "LargeTableBase.h"
#include "SingleRowTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    BUS_CAN_TIME = 0,
    BUS_CAN_SPEED,
    BUS_CAN_INSTANT_FUEL_COMSUPTION,
    BUS_CAN_ENGINE_RPM,
    BUS_CAN_ENGINE_TORQUE,
    BUS_CAN_AIR_CONDITIONOR_STATUS,
    BUS_CAN_TOTAL_FUEL_COMSUPTION,   
    BUS_CAN_TOTAL_MILEAGE,
    BUS_CAN_COLNUM_BUTT
}ENUM_BUS_CAN_COL;

typedef enum
{
    BUS_POS_OCCUR_TIME = 0,
    BUS_POS_DATA,
    BUS_POS_COLNUM_BUTT
}ENUM_BUS_POS_COL;

typedef enum
{
    BUS_PASSENGER_OCCUR_TIME = 0,
    BUS_PASSENGER_STATION_NUMBER,
    BUS_PASSENGER_STATION_NAME,
    BUS_PASSENGER_UP_NUMBER,
    BUS_PASSENGER_DOWN_NUMBER,    
    BUS_PASSENGER_COLNUM_BUTT
}ENUM_BUS_PASSENGER_COL;

typedef enum
{
    BUS_RUN_STATE_DIRECTION = 0,
    BUS_RUN_STATE_BUTT
}ENUM_BUS_RUN_STATE;

class BusCANTable : public LargeTableBase
{
public:
    BusCANTable();
};

class BusPOSTable : public LargeTableBase
{
public:
    BusPOSTable();
};

class BusPassengerTable : public MultiRowWithUniqueIndexTable
{
public:
    BusPassengerTable();
};

class BusRunStateTable : public SingleRowTable
{
public:
    BusRunStateTable();
    void GetDirection(int &dir); // 0: Up 1: Down
};

#endif
