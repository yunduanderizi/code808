

#include "dbusMessage.h"

#define	REQ_ID_TEST	100
#define	REP_ID_TEST	100
#define	NOTF_ID_TEST	100

class TestReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(TestReqC)

public:
	TestReqC():DBusRequestC(REQ_ID_TEST){}
	void print(FILE *fp);

	int	count;
	float	percent;
	char 	buffer[100];
};


class TestRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(TestRepC)

public:
	TestRepC():DBusReplyC(REP_ID_TEST){}
	void print(FILE *fp);

	char buffer[100];
};

class TestNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(TestNotifyC)

public:
	TestNotifyC();
	~TestNotifyC();

	void print(FILE *fp);


	char buffer[100];
	
	char *receiveBuffer;
	char *receiveBuffer2;
	
	struct _internal
	{
		char *receiveBuffer3;
		char *receiveBuffer4;
	}in;
};

