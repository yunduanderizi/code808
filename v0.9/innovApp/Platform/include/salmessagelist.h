#ifndef _SALMESSAGELIST_H_
#define _SALMESSAGELIST_H_

#include "SALDLList.h"
#include "SALMessageNode.h"

class SALMessageList : public SALDLList
{
public:
	SALMessageList();
	virtual ~SALMessageList();
	
	SALMessageNode* pop ();
	void append (SALMessageNode* pNode);		
};

#endif //_SALMESSAGELIST_H_

