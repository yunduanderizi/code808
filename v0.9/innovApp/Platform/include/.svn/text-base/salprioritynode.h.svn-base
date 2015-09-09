#ifndef _SALPRIORITYNODE_H_
#define _SALPRIORITYNODE_H_

#include "SALNode.h"
#include "SALMessageList.h"

class SALPriorityNode : public SALNode
{
public:
	SALPriorityNode(int priority);
	virtual ~SALPriorityNode();
	
	SALMessageList* getList( void );
	int getPriority( void ) const;
private:
	SALMessageList mList;
	const int mPriority;
};

#endif //_SALPRIORITYNODE_H_

