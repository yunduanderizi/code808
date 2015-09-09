#ifndef _SALSCOPELOCK_H_
#define _SALSCOPELOCK_H_

class SALMutex;

/** @brief Scoped locking pattern for mutexes.
    Allows a SALMutex to be locked and unlocked
    with little code overhead. The SALMutex is
    locked when the SSALScopeLock is constructed,
    and is unlocked when it is destroyed (normally
    when the object goes out of scope).
 */

class SALScopeLock
{
public:
    //! class constructor. Mutex will be locked
   	SALScopeLock ( SALMutex& mutex ); 

   	//! class destructor
   	~SALScopeLock ( void );

private:
	// deprecated methods
   	SALScopeLock ( void );
   	SALScopeLock ( const SALScopeLock& );
   	SALScopeLock& operator = ( const SALScopeLock& );

	//! reference to the mutex object
   	SALMutex& mMutex;
};

#endif /*_SALSCOPELOCK_H_*/
