#ifndef TCP_WORK_THREAD_H
#define	TCP_WORK_THREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "phoneMonitorVoiceReportBase.h"
#include "phoneMessage.h"
#define M_BUF_SIZE 64*1024 

typedef struct send_packet
{
	int conNum;
	int len;
	int index;
	unsigned char * msgContent;
	send_packet *next;
}send_packet;


class PhoneMonitorVoiceInterface;
class packetServerThreadC;

class tcpWorkThreadC: public QThread
{
public:
	tcpWorkThreadC();
	~tcpWorkThreadC();

public:
	char connected[LinkNumMax];
	char connected_old[LinkNumMax];
	
	void init(PhoneMonitorVoiceInterface *voicePhone);
	void tcp_connected(int conNum, int state);
	void ip_connected(int ipstate);
	void set_server(int conNum, char *server, int port, int apn);
	int  appendSendMsg(int conNum, unsigned char* packet, int len);
	void data_receive(int conNum);
	void reportTcpStates();
	void data_receive_handler(unsigned char *rxbuffer);
	void connectConfig(TcpReqConfig *configReq);
	

private:
	void run();

	int connectInit();
	int connectTcp(int conNum);
	void processMsg();
	send_packet *getPacket(int conNum, unsigned char *packet, int len)
	{
		send_packet *p_buf;
		p_buf = (send_packet *)malloc(sizeof(send_packet));
		p_buf->msgContent = (unsigned char *)malloc(len);
		memcpy(p_buf->msgContent, packet, len);
		p_buf->len = len;
		p_buf->index = 0;
		p_buf->next = NULL;
		p_buf->conNum = conNum;
		return p_buf;
	}
	void freePacket(send_packet *packet)
	{
		free(packet->msgContent);
		free(packet);
	}
	void clearPacket();
	int  sendReq(int conNum, unsigned char *buf, int len);
	void forwardData(int conNum, unsigned char *buf, int len);
	void forwardStatus(int conNUm, int connected);

private:
	PhoneMonitorVoiceInterface	*m_voicePhone;
	packetServerThreadC *m_server;

private:
	QMutex  	m_mutex; 
	QMutex  	i_mutex; 
	QWaitCondition  m_event;
	//int reconnect;
	int exit;
	int receive_data[LinkNumMax];
	int recvCurNum;
	int linkInit;

	char _server[256];
	int _port;
	int _apn;
	int ipProcess;
	send_packet* sendHead;
	send_packet* sendEnd;
	unsigned char m_sbuf[M_BUF_SIZE];
};

class packetServerThreadC: public QThread
{
public:
	packetServerThreadC();
	~packetServerThreadC();

public:
	void init(tcpWorkThreadC *worker);

private:
	void run();

private:
	tcpWorkThreadC	*m_worker;
	unsigned char m_buf[M_BUF_SIZE];

private:
	int createServer();
	void receiveData(int fd);
	int exit;
};

#endif

