#include "LogUtility.h"
#include "BusRecordTable.h"

const static SQLiteColumn BusCanCols[BUS_CAN_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColOccurTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, true, true),
    SQLiteColumn("ColSpeed",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColInstFuel",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColRpm",             ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColTorque",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColACStatus",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColTotalFuel",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColTotalMileage",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn BusPosCols[BUS_POS_COLNUM_BUTT] = {
    SQLiteColumn("ColOccurTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColData",            ENUM_SQLite_COLUMN_SEQUENCE,     true, false, false, false, false)
};

const static SQLiteColumn BusPassengerCols[BUS_PASSENGER_COLNUM_BUTT] = {
    SQLiteColumn("ColOccurTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColNumber",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColName",            ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false),
    SQLiteColumn("ColUpNum",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColDownNum",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn BusRunStateCols[BUS_RUN_STATE_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColDirection",                ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(-1))
};

BusCANTable::BusCANTable():
                  LargeTableBase(BUS_CAN_DB_NAME,
                                 BUS_CAN_TABLE_NAME, 
                                 (int)BUS_CAN_MAX_ROWS, 
                                 100,
                                 (int)BUS_CAN_COLNUM_BUTT, 
                                 BusCanCols)
{
    ;
}

BusPOSTable::BusPOSTable():
                  LargeTableBase(BUS_POS_DB_NAME,
                                 BUS_POS_TABLE_NAME, 
                                 (int)BUS_POS_MAX_ROWS, 
                                 100,
                                 (int)BUS_POS_COLNUM_BUTT, 
                                 BusPosCols)
{
    ;
}

BusPassengerTable::BusPassengerTable():
                MultiRowWithUniqueIndexTable(BUS_PASSENGER_DB_NAME,
                               BUS_PASSENGER_TABLE_NAME, 
                               (int)BUS_PASSENGER_MAX_ROWS, 
                                (int)BUS_PASSENGER_COLNUM_BUTT, 
                                BusPassengerCols, 
                                (int)BUS_PASSENGER_OCCUR_TIME)

{
  ;
}

BusRunStateTable::BusRunStateTable(): SingleRowTable(BUS_RUN_STATE_DB_NAME, BUS_RUN_STATE_TABLE_NAME, 1, (int)BUS_RUN_STATE_BUTT, BusRunStateCols)
{
    ;
}

void BusRunStateTable::GetDirection(int &dir)
{
    this->Refresh();
    this->GetIntValue(BUS_RUN_STATE_DIRECTION, dir);
}
