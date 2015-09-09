#ifndef _SALUNITTEST_H_
#define _SALUNITTEST_H_

// declare all test classes as friends so that they can access private members
#define DECLARE_FRIEND_SAL_TEST_CLASSES \
	friend class SALThreadTest; \
	friend class SALMutexTest;

#define DECLARE_FRIEND_SAL_THREAD_TEST_CLASSES \
	friend class TestThreadCreationTest;

#endif /*_SALUNITTEST_H_*/
