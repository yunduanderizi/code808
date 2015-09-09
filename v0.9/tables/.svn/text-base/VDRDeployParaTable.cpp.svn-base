#include "VDRDeployParaTable.h"
#include <vector>

const static unsigned char manufactoryID[] = {0x37, 0x30, 0x33, 0x31, 0x39};
#ifdef STUDENT_SWIPE_CARD
const static unsigned char modelNO[] = {0x4a, 0x4b, 0x2d, 0x58, 0x43, 0x30, 0x31, 0x41};
#else
const static unsigned char modelNO[] = {0x4A, 0x4C, 0x39, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif
const static unsigned char modelID[] = {0x32, 0x36, 0x52, 0x30, 0x30, 0x31, 0x38};

const static SQLiteColumn VDRDeployParaCols[VDR_DEPLOY_PARA_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColProvinceID",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(36)),
    SQLiteColumn("ColCityID",               ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(100)),
    SQLiteColumn("ColManufactoryID",        ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false, SQLiteValue(vector<unsigned char>(manufactoryID, manufactoryID + sizeof(manufactoryID)))),
    SQLiteColumn("ColModelNo",              ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false, SQLiteValue(vector<unsigned char>(modelNO, modelNO + sizeof(modelNO)))),
    SQLiteColumn("ColModelID",              ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false, SQLiteValue(vector<unsigned char>(modelID, modelID + sizeof(modelID)))),
    SQLiteColumn("ColPlateColor",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
#ifdef STUDENT_SWIPE_CARD
    SQLiteColumn("ColPlateID",              ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("鄂000000")),
    SQLiteColumn("ColPlateSort",            ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("中型客车")),
#else
    SQLiteColumn("ColPlateID",              ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("赣T00018")),
    SQLiteColumn("ColPlateSort",            ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("大型汽车")),
#endif
    SQLiteColumn("ColCarVIN",               ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("1G1BL52P7TR115520")),
    SQLiteColumn("ColVersion",              ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("0.9.4")),
    SQLiteColumn("ColDeployTime",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1340589036)),
    SQLiteColumn("ColDeployMileage",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1200)),
#ifdef STUDENT_SWIPE_CARD
    SQLiteColumn("ColSpeedPulseFactor",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(5440)),
#else
    SQLiteColumn("ColSpeedPulseFactor",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(3600)),
#endif
    SQLiteColumn("ColLapPulseFactor",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColPhoneNumber",          ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("15679103370")),
    SQLiteColumn("ColAuthToken",            ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("")),
    SQLiteColumn("ColVdrType",              ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(7)),
    SQLiteColumn("ColVdrSimIccid",          ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("")),
    SQLiteColumn("ColHardwareVersion",      ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("1.0.1")),
    SQLiteColumn("ColGnssAttribute",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(3)),
    SQLiteColumn("ColCommunicateAttr",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(9))
};

VDRDeployParaTable::VDRDeployParaTable(): SingleRowTable(VDR_DEPLOY_DB_NAME, VDR_DEPLOY_PARA_TABLE_NAME, 1, (int)VDR_DEPLOY_PARA_BUTT, VDRDeployParaCols)
{
    ;
}
const static SQLiteColumn VDRDeployStatusConfigCols[VDR_DEPLOY_STATUS_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColD0",                   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("保留")),
    SQLiteColumn("ColD1",                   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("保留")),
    SQLiteColumn("ColD2",                   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("保留")),
    SQLiteColumn("ColD3",                   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("近光")),
    SQLiteColumn("ColD4",                   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("远光")),
    SQLiteColumn("ColD5",                   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("右转向")),
    SQLiteColumn("ColD6",                   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("左转向")),
    SQLiteColumn("ColD7",                   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("制动")),
    SQLiteColumn("ColStatusEnable",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0xFF)),
    SQLiteColumn("ColStatusLevel",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0xFF))
};

VDRDeployStatusConfigTable::VDRDeployStatusConfigTable(): SingleRowTable(VDR_DEPLOY_STATUS_CONFIG_DB_NAME, VDR_DEPLOY_STATUS_CONFIG_TABLE_NAME, 1, (int)VDR_DEPLOY_STATUS_BUTT, VDRDeployStatusConfigCols)
{
    ;
}


