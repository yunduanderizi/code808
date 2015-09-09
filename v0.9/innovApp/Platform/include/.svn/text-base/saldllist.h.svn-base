#ifndef _SALDLLIST_H_
#define _SALDLLIST_H_

#include "SALNode.h"

class SALDLList
{
public:
	SALDLList();
	virtual ~SALDLList();
	bool isEmpty() const;
	
	void enqueue ( SALNode* pNode );
	void enqueue ( SALNode* pNode, SALNode* pPrev);
	SALNode* dequeue ( void );
	SALNode* dequeue ( SALNode* pNode );
	SALNode* getHead ( void ) const;
	SALNode* getTail ( void ) const;
	SALNode* getNext ( const SALNode* pNode ) const;
private:
	SALNode mHead;
	SALNode mTail;	
};

#endif //_SALDLLIST_H_

