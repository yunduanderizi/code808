
#include <stdio.h>

#include "dbusClient.h"
#include "dbusApplication.h"
#include "testMessage.h"

#include "dbusDebug.h"

DBusClientC	*client;

void TestNotifyHandler(DBusNotifyC *notify)
{
        if(notify->getIdentifier()!=NOTF_ID_TEST)
                return;

        TestNotifyC *testNotify=(TestNotifyC*)notify;


	printf("notify buffer: %s\n",testNotify->buffer);

	printf("Receive buffer: %s\n",testNotify->receiveBuffer);
	printf("Receive buffer4: %s\n\n",testNotify->in.receiveBuffer4);
}


void* intractThread(void*)
{
       while(1)
       {
             printf("Command(r,n,l):----->");
             //char ch=get(stdin);
	     char buffer[350];
		fgets(buffer,250,stdin);

             switch(buffer[0])
             {
                case 'r':
		{
		 	TestReqC	request;
			strcpy(request.buffer,"Request A Test String");

			DBusReplyC	*reply=NULL;
			int result=client->sendServiceSubThread(&request,reply,5*1000);

			if(result<0)
			{
				printf("Send Service Error. Error Code: %d\n",result);
			}
			else
			{
				if(reply->getIdentifier()==DBUS_REP_ID_COMMON)
				{
					printf("Received Common Reply. result:%d\n",reply->result);
				}
				else if(reply->getIdentifier()==REP_ID_TEST)
				{
					TestRepC *testReply  =(TestRepC*)reply;

					printf("Echo String:%s\n",testReply->buffer);
				}
				else
				{
					printf("Reply not recongnized.\n");
				}

				delete reply;
			}
                      	break;
		}
                case 'n':
		{
		 	TestReqC	request;
			strcpy(request.buffer,"====Test String");

			int result=client->sendService(&request);

			printf("\nReply result: %d\n",result);

                      	break;
		}
                case 'l':
		{
			while(10)
			{
		 		TestReqC	request;
				strcpy(request.buffer,"Test String--loop");

				DBusReplyC	*reply=NULL;
				int result=client->sendServiceSubThread(&request,reply,5*1000);
				if(result<0)
				{
					printf("Send Service Error. Error Code: %d\n",result);
				}
				else
				{
					if(reply->getIdentifier()==DBUS_REP_ID_COMMON)
					{
						printf("Received Common Reply. result:%d\n",reply->result);
					}
					else if(reply->getIdentifier()==REP_ID_TEST)
					{
						TestRepC *testReply  =(TestRepC*)reply;

						printf("Reply buffer: %s\n",testReply->buffer);
					}
					delete reply;
				}
			}
		}
             }
        }

}

int main(int argc,char *argv[])
{
	DBusApplicationC app(argc, argv);

	DBusDebugC::init("testClient");


	unsigned int version =DBusApplicationC::version();
	printf("DBUS version: %d.%d.%d     Release date: %s\n",version>>24,(version>>16)&0xFF,(version & 0xFFFF),DBusApplicationC::releaseDate());

    	client	=new DBusClientC((char*)"com.innov.test", (char*)"/test");


	client->registerReply(REP_ID_TEST,TestRepC::factory);

	client->registerNotify(NOTF_ID_TEST,TestNotifyHandler,TestNotifyC::factory);

	if(client->connect()<0)
        {
                printf("Cann't connect to DBUS server.\n");
                return -1;
        }

/*
    	DBusClientC *client2	=new DBusClientC((char*)"com.innov.test", (char*)"/test");
	client2->registerReply(REP_ID_TEST,TestRepC::factory);
	client2->registerNotify(NOTF_ID_TEST,TestNotifyHandler,TestNotifyC::factory);
	if(client2->connect()<0)
        {
                printf("client 2 Cann't connect to DBUS server.\n");
                return -1;
        }
*/

	pthread_t	threadId;
	pthread_create(&threadId,NULL,intractThread,NULL);

	return app.loop();
}

