
#include "testMessage.h"

void TestReqC::print(FILE *fp)
{
	fprintf(fp,"\tcount=%d\n",count);
	fprintf(fp,"\tpercent=%f\n",percent);
	fprintf(fp,"\tbuffer=[%s]\n\n",buffer);
}

void TestRepC::print(FILE *fp)
{
	fprintf(fp,"\tbuffer=[%s]\n\n",buffer);
}

TestNotifyC::TestNotifyC():DBusNotifyC(REP_ID_TEST)
{
	DBUS_STRING_MEMBER(receiveBuffer)
	DBUS_STRING_MEMBER(receiveBuffer2)
	DBUS_STRING_MEMBER(in.receiveBuffer3)
	DBUS_STRING_MEMBER(in.receiveBuffer4)
}
TestNotifyC::~TestNotifyC()
{
	free(receiveBuffer);
	free(receiveBuffer2);
	free(in.receiveBuffer3);
	free(in.receiveBuffer4);
}
void TestNotifyC::print(FILE *fp)
{
	fprintf(fp,"\tbuffer=[%s]\n",buffer);

	if(receiveBuffer!=NULL)
		fprintf(fp,"\treceiveBuffer=[%s]\n",receiveBuffer);
	else
		fprintf(fp,"\treceiveBuffer=NULL\n");
	
	if(receiveBuffer2!=NULL)
		fprintf(fp,"\treceiveBuffer2=[%s]\n",receiveBuffer2);
	else
		fprintf(fp,"\treceiveBuffer2=NULL\n");
	
	if(in.receiveBuffer3!=NULL)
		fprintf(fp,"\tin.receiveBuffer3=[%s]\n",in.receiveBuffer3);
	else
		fprintf(fp,"\tin.receiveBuffer3=NULL\n");
	
	if(in.receiveBuffer4!=NULL)
		fprintf(fp,"\tin.receiveBuffer4=[%s]\n\n",in.receiveBuffer4);
	else
		fprintf(fp,"\treceiveBuffer=NULL\n\n");
}

