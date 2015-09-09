#include "VDRDregInfoTable.h"

const static SQLiteColumn VDRDregInfoCols[VDR_DREG_INFO_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColActiveDisable",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColEngineStatus",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColLiftStatus",            ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColSpeedLimitStatus",      ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColSpeedLimitValue",       ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColSpeedLimitDuration",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0))
};

VDRDregInfoTable::VDRDregInfoTable(): SingleRowTable(VDR_D310_DREG_DB_NAME, VDR_D310_DREG_TABLE_NAME, 1, (int)VDR_DREG_INFO_BUTT, VDRDregInfoCols)
{
    ;
}

const static SQLiteColumn VDRDregICCols[VDR_DREG_IC_BUTT] = {
    SQLiteColumn("ColOccurTime",            ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false),
    SQLiteColumn("ColDriverLicenseNum",     ENUM_SQLite_COLUMN_STRING,     true, false, false, false, true)
};

VDRDregICTable::VDRDregICTable():
    MultiRowWithUniqueIndexTable(VDR_D310_DREG_IC_DB_NAME,
                                 VDR_D310_DREG_IC_TABLE_NAME, 
                                 (int)VDR_D310_DREG_IC_MAX_ROWS, 
                                 (int)VDR_DREG_IC_BUTT, 
                                 VDRDregICCols, 
                                 (int)VDR_DREG_IC_LICENSE_NUM)
{
    ;
}

