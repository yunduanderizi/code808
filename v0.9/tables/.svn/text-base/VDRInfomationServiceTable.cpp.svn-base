#include "VDRInfomationServiceTable.h"

const static SQLiteColumn VdrInfomationsServiceCols[VDR_INFOMATIONS_SERVICE_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColTime",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColType",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLen" ,        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColContents",    ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false),
};

VDRInfomationServiceTable::VDRInfomationServiceTable() :
    MultiRowWithUniqueIndexTable(VDR_INFOMATIONS_SERVICE_DB_NAME,
                                 VDR_INFOMATIONS_SERVICE_TABLE_NAME, 
                                 (int)VDR_INFOMATIONS_SERVICE_TABLE_MAX_ROWS, 
                                 (int)VDR_INFOMATIONS_SERVICE_COLNUM_BUTT, 
                                 VdrInfomationsServiceCols, 
                                 (int)VDR_INFOMATIONS_SERVICE_TIME)

{
    ;
}
