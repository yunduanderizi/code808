#include "VDRTextRecordTable.h"

const static SQLiteColumn VdrTextsCols[VDR_TEXTS_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColTime",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("Colflag",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColContents",    ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false),
};

VDRTextRecordTable::VDRTextRecordTable() :
    MultiRowWithUniqueIndexTable(VDR_TEXTS_DB_NAME,
                                 VDR_TEXTS_TABLE_NAME, 
                                 (int)VDR_TEXTS_TABLE_MAX_ROWS, 
                                 (int)VDR_TEXTS_COLNUM_BUTT, 
                                 VdrTextsCols, 
                                 (int)VDR_TEXTS_TIME)

{
    ;
}
