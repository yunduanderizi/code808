#ifndef _VDR_QUESTION_TABLE_H_
#define _VDR_QUESTION_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_QUESTIONS_TIME= 0,
    VDR_QUESTIONS_SEQ,
    VDR_QUESTIONS_FLAG,
    VDR_QUESTIONS_LEN,
    VDR_QUESTIONS_QUESTION,
    VDR_QUESTIONS_ANSWER_SUM,
    VDR_QUESTIONS_ANSWER_LIST,
    VDR_QUESTIONS_COLNUM_BUTT
}ENUM_VDR_QUESTIONS_COL;

class VDRQuestionsTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRQuestionsTable();
};


#endif
