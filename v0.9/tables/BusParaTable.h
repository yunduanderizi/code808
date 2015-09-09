#ifndef _SETTING_DEPLOY_SQLITE_TABLE_H_
#define _SETTING_DEPLOY_SQLITE_TABLE_H_

#include "SingleRowTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    BUS_RUN_PARA_PASSENGER_DATA_UPLOAD_INTERVAL = 0,
    BUS_RUN_PARA_DRIVING_DATA_UPLOAD_INTERVAL,
    BUS_RUN_PARA_LED_MODE,
    BUS_RUN_PARA_LED_SPEED,
    BUS_RUN_PARA_LED_LOOP_TIMES,
    BUS_RUN_PARA_BUTT
}ENUM_BUS_RUN_PARA_COL;

class BusRunParaTable : public SingleRowTable
{
public:
    BusRunParaTable();
};

typedef enum
{
    BUS_DEPLOY_PARA_LED_NUMBER = 0,
    BUS_DEPLOY_PARA_ROUTE_NAME,
    BUS_DEPLOY_PARA_START_STATION,
    BUS_DEPLOY_PARA_TERMINAL_STATION,
    BUS_DEPLOY_PARA_POS_PASSWORD,
    BUS_DEPLOY_PARA_BUTT
}ENUM_BUS_DEPLOY_PARA_COL;

class BusDeployParaTable : public SingleRowTable
{
public:
    BusDeployParaTable();
};

#endif
