#include "BusParaTable.h"

const static SQLiteColumn BusRunParaCols[BUS_RUN_PARA_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColPassengerDataInterval",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(60)),
    SQLiteColumn("ColDrivingDataInterval",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColLedMode",                  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColLedSpeed",                 ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColLedLoopTimes",             ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(2))
};

BusRunParaTable::BusRunParaTable(): SingleRowTable(BUS_RUN_PARA_DB_NAME, BUS_RUN_PARA_TABLE_NAME, 1, (int)BUS_RUN_PARA_BUTT, BusRunParaCols)
{
    ;
}

const static unsigned char pos_pass[] = {0x38, 0x30, 0x30, 0x37, 0x30, 0x34};

const static SQLiteColumn BusDeployParaCols[BUS_DEPLOY_PARA_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColLedNumber",                ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(4)),
    SQLiteColumn("ColRouteName",                ENUM_SQLite_COLUMN_STRING,     true, false, false, false, false, SQLiteValue("")),
    SQLiteColumn("ColStartStation",             ENUM_SQLite_COLUMN_STRING,     true, false, false, false, false, SQLiteValue("")),
    SQLiteColumn("ColTerminalStation",          ENUM_SQLite_COLUMN_STRING,     true, false, false, false, false, SQLiteValue("")),
    SQLiteColumn("ColPOSPassword",              ENUM_SQLite_COLUMN_SEQUENCE,     true, false, false, false, false, SQLiteValue(vector<unsigned char>(pos_pass, pos_pass + sizeof(pos_pass)))),
};

BusDeployParaTable::BusDeployParaTable(): SingleRowTable(BUS_DEPLOY_PARA_DB_NAME, BUS_DEPLOY_PARA_TABLE_NAME, 1, (int)BUS_DEPLOY_PARA_BUTT, BusDeployParaCols)
{
    ;
}
