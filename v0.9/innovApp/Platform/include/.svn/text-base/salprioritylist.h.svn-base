#ifndef _SALPRIORITYLIST_H_
#define _SALPRIORITYLIST_H_

#include "SALDLList.h"
#include "SALPriorityNode.h"


class SALPriorityList : public SALDLList
{
public:
	SALPriorityList();
	virtual ~SALPriorityList();

	SALPriorityNode* pop ();
	SALPriorityNode* getList (const int pPriority);
private:
	bool insert (const int pPriority);
	void remove (const int pPriority);

	SALPriorityNode* mPriorityList[SAL_MAX_PRIORITY];
};

#endif //_SALPRIORITYLIST_H_

