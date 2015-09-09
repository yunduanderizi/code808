#ifndef _SAL_DEBUGTRACE_ADDON_H_
#define _SAL_DEBUGTRACE_ADDON_H_

/** @file SALDebugTraceAddon.h
  * @brief Extra features for DebugTrace in C++
  */

/** Automatically generate debug-trace output when the constructor and
  * destructor are called allowing the scope to be traced.
  */
class ScopeInspector
{
public:
	ScopeInspector ( const char* file, int line, byte_t group, const char* str );
   ~ScopeInspector ( void );
   
private:
	const char* 	mFilename;
	int 			mLineNumber;
	byte_t 			mGroup;
	const char* 	mBuffer;
};

/** Code should not use ScopeInspector:: directly, but rather use this define.
  */
#define INSPECT_SCOPE(group,infostr) \
	ScopeInspector scopeInspector(__FILE__,__LINE__,group,infostr);


#endif /*_SAL_DEBUGTRACE_ADDON_H_*/
