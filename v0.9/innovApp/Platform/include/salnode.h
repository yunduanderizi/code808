#ifndef _SALNODE_H_
#define _SALNODE_H_

#include "SALTypes.h"

class SALNode
{
public:
	SALNode();
	virtual ~SALNode();
	SALNode* next( void ) const { return mNext; }
	SALNode* prev( void ) const { return mPrev; }
	void setNext(SALNode* pNode) { mNext = pNode; }
	void setPrev(SALNode* pNode) { mPrev = pNode; }
private:
	//! previous node
	SALNode* mPrev;

	//! next node
	SALNode* mNext;
};

#endif //_SALNODE_H_

