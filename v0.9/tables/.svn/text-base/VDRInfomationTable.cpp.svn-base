#include "VDRInfomationTable.h"

const static SQLiteColumn VdrInfomationsCols[VDR_INFOMATION_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColId",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColTime",         ENUM_SQLite_COLUMN_INTEGER,      true, false, false, false,false),
    SQLiteColumn("ColName",         ENUM_SQLite_COLUMN_STRING,      true, false, false, false, true),
    SQLiteColumn("ColFlag",         ENUM_SQLite_COLUMN_INTEGER,      true, false, false, false,false),
    SQLiteColumn("ColContents",     ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false),   
};

VDRInfomationTable::VDRInfomationTable() :
    MultiRowWithUniqueIndexTable(VDR_INFOMATION_DB_NAME,
                                 VDR_INFOMATION_TABLE_NAME, 
                                 (int)VDR_INFOMATION_TABLE_MAX_ROWS, 
                                 (int)VDR_INFOMATION_COLNUM_BUTT, 
                                 VdrInfomationsCols, 
                                 (int)VDR_INFOMATION_TYPE)

{
    ;
}
