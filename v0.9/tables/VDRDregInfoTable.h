#ifndef _VDR_DREG_INFO_TABLE_H_
#define _VDR_DREG_INFO_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "SingleRowTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_DREG_ACTIVE_DISABLE = 0,
    VDR_DREG_ENGINE_STATUS,
    VDR_DREG_LIFT_STATUS,
    VDR_DREG_SPEED_LIMIT_STATUS,
    VDR_DREG_SPEED_LIMIT_VALUE,
    VDR_DREG_SPEED_LIMIT_DURATION,
    VDR_DREG_INFO_BUTT
}ENUM_VDR_DREG_INFO_COL;

class VDRDregInfoTable : public SingleRowTable
{
public:
    VDRDregInfoTable();
};


typedef enum
{
    VDR_DREG_IC_OCCUR_TIME = 0,
    VDR_DREG_IC_LICENSE_NUM,
    VDR_DREG_IC_BUTT
}ENUM_VDR_DREG_IC_COL;

class VDRDregICTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRDregICTable();
};


#endif
