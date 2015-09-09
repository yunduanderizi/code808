#include "VDRQuestionTable.h"

const static SQLiteColumn VdrQuestionsCols[VDR_QUESTIONS_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColTime",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColSeq",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColFlag",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLen",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColQuestion",    ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false),
    SQLiteColumn("ColAnswersum",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColAnswerslist", ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
};

VDRQuestionsTable::VDRQuestionsTable() :
    MultiRowWithUniqueIndexTable(VDR_QUESTIONS_DB_NAME,
                                 VDR_QUESTIONS_TABLE_NAME, 
                                 (int)VDR_QUESTIONS_TABLE_MAX_ROWS, 
                                 (int)VDR_QUESTIONS_COLNUM_BUTT, 
                                 VdrQuestionsCols, 
                                 (int)VDR_QUESTIONS_TIME)

{
    ;
}
