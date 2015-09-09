#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include <QMutexLocker> 
#include "debugMonitor.h"
#include "tcpWorkThread.h"
#include "MobileInterFace.h"
#include "phoneConnection.h"
#include "phoneMessage.h"

#define RX_TIMEOUT_TIME 120  //s
static int debug = 1;
#define dprintf(msg...) if(debug) {printf(msg);}

static int arrears, powerdw;
char server_path[6][30] = {
	PHONE_DATA_SERVER_PATH2,
	PHONE_DATA_SERVER_PATH3,
	NULL,
	NULL,
	NULL,
	NULL,
};

tcpWorkThreadC::tcpWorkThreadC()
{
	sendHead=NULL;
	sendEnd = NULL;
	ipProcess = -2;
	//reconnect = 0;
	//connected = 0;
	linkInit = 0;
	_apn = APN_CMNET;
	//receive_data = 0;
	memset(receive_data, 1, sizeof(receive_data));
	_server[0] = 0;
	exit = 0;
	memset(connected, 255, sizeof(connected));
	connected[0] = 111;  //add for sleep_start mode
	connected[1] = 111;
	memset(connected_old, 255, sizeof(connected_old));
	m_server = new packetServerThreadC();
}

tcpWorkThreadC::~tcpWorkThreadC()
{
	exit = 1;
	m_event.wakeOne();
	delete m_server;
	clearPacket();
}


void tcpWorkThreadC::clearPacket()
{
	QMutexLocker locker(&m_mutex);
	while(sendHead)
	{
		send_packet *tmp = sendHead;
		sendHead = sendHead->next;
		freePacket(tmp);
	}
	sendEnd = NULL;
}

void tcpWorkThreadC::init(PhoneMonitorVoiceInterface *voicePhone)
{
	m_voicePhone =voicePhone;
	start();
	m_server->init(this);
}
void tcpWorkThreadC::ip_connected(int ipstate)
{
	QMutexLocker locker(&m_mutex);
	
	ipProcess = ipstate;

	//m_event.wakeOne();
}


void tcpWorkThreadC::tcp_connected(int conNum, int state)
{
	QMutexLocker locker(&m_mutex);
	connected[conNum] = state;
	m_event.wakeOne();
}

void tcpWorkThreadC::data_receive(int conNum)
{
	QMutexLocker locker(&m_mutex);
	if(conNum >= LinkNumMax)
		return;
	receive_data[conNum] = true;
	//recvCurNum = true;
	m_event.wakeOne();
}

void tcpWorkThreadC::set_server(int conNum, char *server, int port, int apn)
{
	printf("\n\n#############LinkInit:[%d] conNum: [%d], APN:[%d], Server:[%s], Port:[%d]############\n\n",
			linkInit, conNum, apn, server, port);
	QMutexLocker locker(&i_mutex);
	bool waitResult;
	memset(_server, 0, sizeof(_server));
	strncpy(_server, server, sizeof(_server) - 1);
	_port = port;
	_apn = apn;
	//if((port == 1111) && (strcmp(server, "1.1.1.1") == 0) ){
	//	m_voicePhone->closeLink();
	//	linkInit = 0;
	//}
	//reconnect = 1;
	if(!linkInit){
		waitResult = m_event.wait(&i_mutex,10*1000);
		printf("###############waitReslt: %d################\n", waitResult);
		if(!waitResult || !linkInit){
			forwardStatus(conNum, 0);
			return;
		}
	}
	connectTcp(conNum);
	m_event.wakeOne();
}

void tcpWorkThreadC::connectConfig(TcpReqConfig *configReq)
{
	printf("###############connectConfig conNum[%d], closelink[%d], closetcp[%d]##############\n", 
			configReq->hdr.conNum, configReq->closelink, configReq->closetcp);
	if(configReq->closelink){
		m_voicePhone->closeLink();
		linkInit = 0;
	}
	if(configReq->closetcp)
		m_voicePhone->closeTcp(configReq->hdr.conNum);
}
int tcpWorkThreadC::connectInit()
{
	while(!exit)
	{
		if(powerdw > 4){
			powerdw = 0;
			m_voicePhone->setPowerDown(1);
			break;
		}

		if(arrears > 100){
			powerdw++;
			m_voicePhone->setCfun(1, 1, 5000);
			arrears = 0;
			break;
		}

		m_voicePhone->closeLink();
		m_mutex.lock();
		m_event.wait(&m_mutex,1*1000);
		m_mutex.unlock();

		arrears++;;

		if(m_voicePhone->getGprs() == 0)
			continue;

		if(m_voicePhone->setMux(1) < 0)
			continue;

		if(m_voicePhone->setRxMode() < 0)
			continue;

		//if(m_voicePhone->setTransMode(0) < 0)
		//	continue;

		if(m_voicePhone->setFastSend(1) < 0)
			continue;

		if(m_voicePhone->setAPN(_apn) < 0)
			continue;
		

		if(m_voicePhone->connectLink() < 0)
			continue;

		if(m_voicePhone->get_LocalIP() < 0)
			continue;
		arrears = 0;
		linkInit = 1;
		m_event.wakeOne();
		ip_connected(2);
		return 0;
	}	
	return -1;
}
int tcpWorkThreadC::connectTcp(int conNum)
{
	bool waitResult;
	int ret;
	ret = 0;
	//while(!exit)
	//{
		printf("\n\n@@@@@@@@@@@@@@@@@@@@@@@@@@ TCP Con[[%d]] Connecting ... \n", conNum);
		//If TCP has connected when starting by sleep mode   KKKKKeep it... 
		//if(!reconnect && (m_voicePhone->getTcpStatus(conNum) == 1)){   
		//	goto CONNECTED;
		//}
		
		if(!linkInit){
			ret = -1;
			goto CONNECTFAIL;
			//break;
		}
		if(arrears > 100){
			//if(conNum == 0){
			m_voicePhone->closeLink();
			linkInit = 0;
			//}
			//arrears = 0;
			ret = -2;
			goto CONNECTFAIL;
			//break;
		}
		if(connected[0] == 1 || connected[1] == 1){
			arrears = 0;
		}
		else{
			arrears++;
		}
		
		//dprintf("############arrears=%d###############\n", ipProcess,arrears);
		//if(connected[conNum] == 1) {
		m_voicePhone->closeTcp(conNum);
		//}

		//reconnect = 0;
		connected[conNum] = false;
		//forwardStatus(conNum, 0);

		//m_mutex.lock();
		//waitResult = m_event.wait(&m_mutex,1*1000);
		//m_mutex.unlock();
		//if(reconnect)
		//	break;
		
		if(!_server[0]){
			ret = -3;
			goto CONNECTFAIL;
		}
		if(m_voicePhone->connectTcp(conNum, _server, _port) < 0){
			ret = -4;
			goto CONNECTFAIL;
		}
		//if(waitResult)// || !connected)
		//	continue;
		arrears = 0;
		//clearPacket();
		//tcp_connected(1);
		connected[conNum] = true;
		connected_old[conNum] = true;
		forwardStatus(conNum, 1);
		printf("\n\n@@@@@@@@@@@@@@@@@ TCP Con[[%d]] connect successfully...\n", conNum);
		//reconnect = 0;
		return 0;


CONNECTFAIL:
		connected[conNum] = false;
		connected_old[conNum] = false;
		forwardStatus(conNum, 0);
		printf("\n\n@@@@@@@@@@@@@@@@@ TCP Con[[%d]] connect fail error:[%d]...\n", conNum, ret);
		return ret;
}

int tcpWorkThreadC::sendReq(int conNum, unsigned char *buf, int buflen)
{
    int fd, addrlen, len;
    struct sockaddr_un addr;
    int ret;

	if(!server_path[conNum])
		return -1;

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(fd < 0) {
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    len = sizeof(server_path[conNum]);
    memcpy(addr.sun_path, server_path[conNum], len);
    addrlen = len + 2;

    ret = sendto(fd, buf, buflen, MSG_DONTWAIT, (const sockaddr*)&addr, addrlen);
    //ret = sendto(fd, buf, buflen, 0, (const sockaddr*)&addr, addrlen);
    if(ret != buflen)
        return -1;

    close(fd);
    return 0;
}

void tcpWorkThreadC::forwardData(int conNum, unsigned char *buf, int len)
{

	TcpReqHdr *hdr = (TcpReqHdr *)m_sbuf;
	hdr->msgid = TCP_REQ_TYPE_DATA;
	hdr->msglen = len;
	hdr->conNum = conNum;
	memcpy(m_sbuf + sizeof(TcpReqHdr), buf, len);
	
	if(debug){
		int i;
		printf("\n\nforwardData[Con%d]:[%d]<<== ", conNum, len);
		for(i = 0; i<len+sizeof(TcpReqHdr); i++){
			printf("%02X ", m_sbuf[i]);
		}
		printf("\n\n");
	}
	sendReq(conNum, m_sbuf, sizeof(TcpReqHdr)+len);
	//sendReq(conNum, (unsigned char *)&hdr, sizeof(TcpReqHdr));
	//sendReq(conNum, buf, len);
}

void tcpWorkThreadC::forwardStatus(int conNum, int connected)
{
	printf("\n#########forwardStatus conNum:[%d] connected:[%d]############\n", conNum, connected);
	TcpReqStatus statusReq;
	statusReq.hdr.msgid = TCP_REQ_TYPE_STATUS;
	statusReq.hdr.msglen = sizeof(TcpReqStatus) - sizeof(TcpReqHdr);
	statusReq.hdr.conNum = conNum;
	statusReq.connected = connected;
	sendReq(conNum, (unsigned char *)&statusReq, sizeof(TcpReqStatus));
	//sendReq(conNum, (unsigned char *)&statusReq, sizeof(TcpReqHdr));
	//sendReq(conNum, (unsigned char *)&statusReq.connected, sizeof(int));
	m_voicePhone->m_pCallback->toTcpState(connected);
}


void tcpWorkThreadC::reportTcpStates()
{
	int i; 
	for(i = 0; i < LinkNumMax; i++){
		if(connected[i] != 255){
			connected[i] = m_voicePhone->getTcpStatus(i);
			if(connected[i] != connected_old[i]){
				forwardStatus(i, connected[i]);
				connected_old[i] = connected[i];
			}
		}
	}
}

void tcpWorkThreadC::data_receive_handler(unsigned char *rxdata)
{
	int i,ret;
	for(i = 0; i < LinkNumMax; i++){
		if(connected[i] == 255)
			continue;
		ret = receive_data[i];
		while(ret)
		{
			ret = m_voicePhone->getRxData(i, rxdata, 1024);
			if(ret)
				forwardData(i, rxdata, ret);
		}
		receive_data[i] = 0;
		//recvCurNum = false;
		
	}
}

void tcpWorkThreadC::processMsg()
{
	int send_length; 
	int ret;
	int times = RX_TIMEOUT_TIME;
	char recvBuffer[1000];
	unsigned char rxdata[1024];

	while(!exit && linkInit)
	{
		m_mutex.lock();
		m_event.wait(&m_mutex,1*1000);
		m_mutex.unlock();
		
		//reportTcpStates();

		if(1 ==ipProcess){
			continue;
		}else if(0 == ipProcess){
			linkInit = 0;
			break;
		}

		//if(!recvCurNum){times=RX_TIMEOUT_TIME;}else{times--;}
		times--;

		if(!times){
			memset(receive_data, 1, sizeof(receive_data));
			//recvCurNum = true;
			times = RX_TIMEOUT_TIME;
		}


		data_receive_handler(rxdata);

		if(!sendHead)
			continue;

		//if((send_length = m_voicePhone->getSendLength()) <= 0)
		//	continue;

		send_length = 1460;

		//while(sendHead && send_length)
		while(sendHead)
		{
			if(connected[sendHead->conNum] == 0){
				printf("\n##############Droppppppppppppppp it --- connected[%d]=%d################\n", 
						sendHead->conNum, connected[sendHead->conNum]);
				forwardStatus(sendHead->conNum, 0);
				m_mutex.lock();
				send_packet *tmp = sendHead;
				sendHead = sendHead->next;
				if(!sendHead)
					sendEnd = NULL;
				m_mutex.unlock();
				freePacket(tmp);
				continue;
			}
			int txlen = sendHead->len - sendHead->index;
			if(txlen > send_length)
				txlen = send_length;
			
			dprintf("\n\nNew Packet: txlen[%d] len[%d] sendlen[%d] index[%d] conNum[%d]\n\n",
					txlen, sendHead->len,send_length, sendHead->index, sendHead->conNum);

			if(debug){
				printf("\n\nSendData[Con%d]:[%d] ===> ", sendHead->conNum, txlen);
				int i;
				for(i=0; i<txlen; i++){
					printf("%02X ", *((sendHead->msgContent + sendHead->index)+i));
				}
				printf("\n\n");
			}

			ret = m_voicePhone->m_pPhoneConnection->sendTCPCommand(sendHead->conNum, sendHead->msgContent + sendHead->index,
					txlen, recvBuffer, 1000, 35000);

			if(ret>=0 && strstr(recvBuffer, "DATA ACCEPT") != NULL)
			{
				//send_length -= txlen;
				sendHead->index += txlen;
				txlen = sendHead->len - sendHead->index;
			}

			if(!txlen)
			{
				m_mutex.lock();
				send_packet *tmp = sendHead;
				sendHead = sendHead->next;
				if(!sendHead)
					sendEnd = NULL;
				m_mutex.unlock();
				freePacket(tmp);
			}

			m_mutex.lock();
			m_event.wait(&m_mutex,1*1000);  //wait data come in
			m_mutex.unlock();

			if(!linkInit){
				clearPacket();
				break;
			}
			
			data_receive_handler(rxdata);
		}
	}
}

void tcpWorkThreadC::run()
{
	while(!exit)
	{
		m_mutex.lock();
		m_event.wait(&m_mutex, 5*1000);
		m_mutex.unlock();
		
		//Query data initiativly
		memset(receive_data, 1, sizeof(receive_data));

		reportTcpStates();
		//If TCP Con has connected when starting by sleep mode KKKKKeep it... 
		//if(m_voicePhone->getTcpStatus(0) == 1){   
		if(connected[0] || connected[1]){   
			//connected[0] = 1;
			linkInit = 1;
			m_event.wakeOne();
		}else{
			//connected[0] = 0;
			linkInit = 0;
			if(connectInit() < 0)
				continue;
		}
		processMsg();
		
	
	}	
}

int tcpWorkThreadC::appendSendMsg(int conNum, unsigned char* packet, int len)
{
	if(connected[conNum] != 1){
		dprintf("###########appendSendMsg: conNum[%d] is offline[%d], Drop ittttttt###########\n", 
				conNum, connected[conNum]);
		return -1;
	}
	QMutexLocker locker(&m_mutex);

	send_packet *p_buf = getPacket(conNum, packet, len);

	if(sendHead== NULL)
	{
		sendHead = sendEnd = p_buf;
	}
	else
	{
		sendEnd->next = p_buf;
		sendEnd = p_buf;
	}

	m_event.wakeOne();

	return 0;
}

packetServerThreadC::packetServerThreadC()
{
	m_worker=NULL;
	exit = 0;
}

packetServerThreadC::~packetServerThreadC()
{
	exit = 1;
}

void packetServerThreadC::init(tcpWorkThreadC *worker)
{


	m_worker =worker;
	start();
}

void packetServerThreadC::receiveData(int fd)
{
	int len;
	TcpReqHdr *hdr;
	TcpReqConnect *connReq;
	TcpReqConfig *configReq;
	int conNum, apn, port, iplen;
    char ip[256];
	while(1)
	{
		len = recv(fd, m_buf, M_BUF_SIZE, 0);
		if(len <= 0) {
			break;
		}
		hdr = (TcpReqHdr *)m_buf;
		conNum = hdr->conNum;
		//dprintf("\n########%s:Con[%d], msgid[%d], msglen[%d] len[%d]#######\n", __func__,
		//		conNum, hdr->msgid, hdr->msglen, len);
		if(conNum >= LinkNumMax){
			printf("=======ConNum%d >= LinkNumMax%d Drop it=======\n", conNum, LinkNumMax);
			continue;
		}

		if(m_worker->connected[conNum] == 255)
			m_worker->connected[conNum] = 0;
		//if(hdr->msglen)
		//  len = recv(fd, m_buf + sizeof(TcpReqHdr), hdr->msglen, 0);

		if(0){
			printf("\n\nrecvData[Con%d]:[%d] ===> ", hdr->conNum,  hdr->msglen);
			for(int i=0; i< hdr->msglen+sizeof(TcpReqHdr); i++){
				printf("%02X ", *(m_buf+i));
			}
			printf("\n\n");
		}

		switch(hdr->msgid)
        {
        case TCP_REQ_TYPE_DATA:
        	if(hdr->msglen)
        		m_worker->appendSendMsg(conNum, m_buf + sizeof(TcpReqHdr), hdr->msglen);
        	break;
        case TCP_REQ_TYPE_CONNECT:
            connReq = (TcpReqConnect *)m_buf;
            port = connReq->port;
			apn = connReq->apn;
            iplen = hdr->msglen - sizeof(unsigned int)*2;
            if((iplen <= 0) || (iplen >= (int)sizeof(ip)))
            	break;
            memcpy(ip, connReq->ip, iplen);
            ip[iplen] = 0;
            m_worker->set_server(conNum, ip, port, apn);
            break;
        case TCP_REQ_TYPE_CONFIG:
            configReq = (TcpReqConfig *)m_buf;
			m_worker->connectConfig(configReq);
            break;
        default:
        	break;
        }
	}
}

void packetServerThreadC::run()
{
    int fd;

    while(1)
    {
    	if((fd = createServer()) < 0)
    		return;
    	receiveData(fd);
    	close(fd);
    }
}

int packetServerThreadC::createServer()
{
	int fd, addrlen, len;
	struct sockaddr_un addr;
	int on = 1;

	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(fd < 0) {
		return -1;
	}

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	len = sizeof(PHONE_DATA_SERVER_PATH);
	memcpy(addr.sun_path, PHONE_DATA_SERVER_PATH, len);
	addrlen = len + 2;

	if(bind(fd, (struct sockaddr *)&addr, addrlen) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}
