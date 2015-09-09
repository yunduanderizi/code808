#ifndef _SALMESSAGEPOOL_H_
#define _SALMESSAGEPOOL_H_

#include "SALTypes.h"

class SALMutex;

/** @brief Manage a pool of messages.
  * 
  * The maximum size of a message is defined in SALCustomize.h.
  * The SALRawMessage_t is defined in SALTypes.h and is normally
  * a byte_t*.
  */

class SALMessagePool
{
public:
    //! create the message pool (only one pool per application)
    //! @return true if the pool was successfully created
    static bool create  ( uint16_t numOfMessages );
    
    //! destroy the message pool
    //! @return true if pool was deleted
    static bool destroy ( void );
    
    //! get a pointer to the message pool instance
    //! @return pointer to the one message pool
    static SALMessagePool* getInstance ( void ); 
    
    //! retrieve a message buffer from the bool
    //! @param[out] id fills in the message identifier
    //! @param[out] pMsgPtr fills in the pointer to the message buffer
    //! @return true if a message was allocated 
    bool allocateMessageBuffer ( int16_t* id, SALRawMessage_t* pMsgPtr );
    
    //! release a message buffer in the pool
    //! @param id message identifier to be freed
    //! @return true if message was successfully freed
    bool freeMessageBuffer ( int16_t id );
    
    //! get the slot identified by the specified ID
    //! @param id message identifier
    //! @return poitner to message buffer
    SALRawMessage_t getMessageBuffer ( int16_t id );
        
private:
    //! protected class constructor accessed by the singleton constructor
    SALMessagePool ( uint16_t numOfMessages );
    
    //! protected class destructor accessed by the singleton destructor
    virtual ~SALMessagePool ( void );
  
private:
    //! pool of raw message (allocated on the heap here)
    SALRawMessage_t* mMessagePool;
    
    //! size of messages in the pool
    uint16_t mSize;
    
    //! ring buffer with IDs of empty slots
    int16_t* mFreeList;
    int16_t  mFreeSlots;
    int16_t  mHead;
    int16_t  mTail;

    int16_t  mPeakLevel;
    
    //! mutex for synchronized access
    static SALMutex* mMutex;
    
    //! pointer to the singleton instance
    static SALMessagePool* mpInstance;
};

#endif /*_SALMESSAGEPOOL_H_*/
