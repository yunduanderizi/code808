
#include <QtGui/QApplication>


#include "dbusMessage.h"
#include "dbusApplication.h"
#include "dbusServer.h"
#include "testMessage.h"

#include "dbusDebug.h"

DBusServerC	*pServer;

int	no=0;
void TestServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
	if(request->getIdentifier()!=REQ_ID_TEST)
		return;

	TestReqC *testRequest=(TestReqC*)request;

	TestRepC *testReply	=new TestRepC();

	strcpy(testReply->buffer,testRequest->buffer);
	testReply->result =no++;

	reply=testReply;

	TestNotifyC	notify;
	strcpy(notify.buffer,testRequest->buffer);

	notify.receiveBuffer=(char*)malloc(1024*1024);

	for(int i=0;i<1024;i++)
	{
		if(i%5==0)
			notify.receiveBuffer[i]='F';
		else if(i%11==0)
			notify.receiveBuffer[i]='A';
		else
			notify.receiveBuffer[i]='T';
	}
	notify.receiveBuffer2=strdup("kkkkkkkkkkkkkkkkk========bwy");

	notify.in.receiveBuffer4=strdup("-------How are you-----========HI");

	pServer->sendNotify(&notify);	
}

int main(int argc,char *argv[])
{
	QApplication app(argc, argv);

	DBusDebugC::init("testServer");

	unsigned int version =DBusApplicationC::version();
        printf("DBUS version: %d.%d.%d     Release date: %s\n",version>>24,(version>>16)&0xFF,(version & 0xFFFF),DBusApplicationC::releaseDate());


	DBusServerC	server((char*)"com.innov.test",(char*)"/test");	
	
	pServer	=&server;

	server.registerService(REQ_ID_TEST,TestServiceHandler,TestReqC::factory);

	if(server.connect()<0)
	{
		printf("Cann't connect to DBUS server.\n");
		return -1;
	}


	return app.exec();
}

