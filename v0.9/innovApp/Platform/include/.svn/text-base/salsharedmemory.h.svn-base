#ifndef SALSHAREDMEMORY_H_
#define SALSHAREDMEMORY_H_

#if defined(LINUX)||defined(QNX)
// Define O_CREAT etc.
#include <fcntl.h>
#define SAL_SHM_RDONLY	O_RDONLY
#define SAL_SHM_WRONLY	O_WRONLY
#define SAL_SHM_RDWR	O_RDWR
#define SAL_SHM_CREAT	O_CREAT
#define SAL_SHM_EXCL	O_EXCL

#else

#define SAL_SHM_RDONLY	00
#define SAL_SHM_WRONLY	01
#define SAL_SHM_RDWR	02
#define SAL_SHM_CREAT	0100
#define SAL_SHM_EXCL	0200

#endif



class SALSharedMemory
{
public:
#if defined(WIN32) || defined(_WIN32_WCE)
	SALSharedMemory ( const TCHAR* name, int flags, int memSize = -1 );
#else
	SALSharedMemory ( const char* name, int flags, int memSize = -1 );
#endif
	virtual ~SALSharedMemory ( void );

	//! map the memory
	void*  mapMemory ( void* startAddr, int byteSize, int protection, int flags );

protected:
	//! create the shared memory resource
	bool create ( int32_t memSize = -1 );

	//! destroy the shared memory resource
	bool destroy ( void );

protected:
#if defined(WIN32) || defined(_WIN32_WCE)
	//! name of the shared memory file
	const TCHAR* mName;

	//! handle of the memmory map file
	HANDLE mMemoryHandle;
#else
	//! name of the shared memory file
	const char*	 mName;

	//! handle of the memmory map file
	int  mMemoryHandle;
#endif
	
	//! shared memory properties
	int  mProperties;
	
	//! mapping address after the memory was mapped
	void*  mMemoryPtr;

	//! size of the mapped memory area
	int32_t mByteSize;
	
	//! flag to indicate whether the mapping was done
	bool mIsMemoryMapped;
};

#endif /*SALSHAREDMEMORY_H_*/
