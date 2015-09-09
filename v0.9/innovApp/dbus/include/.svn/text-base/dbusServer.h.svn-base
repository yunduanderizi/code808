#ifndef DBUS_SERVER_H_20100617
#define DBUS_SERVER_H_20100617

#include <pthread.h>

#include "dbusMessage.h"

class QByteArray;
class QDBusConnection;
class DBus_extendAdaptorImp;
class DBus_extendAdaptor;
class  DBusNotifyThreadC;
struct _ServiceItem;


typedef DBusRequestC*   (*RequestFactory)(void);
typedef void (*ServiceHandler)(DBusRequestC *request, DBusReplyC *&reply);

class DBusServerC
{
public:
	DBusServerC(char *serverName,char *objectName);
	~DBusServerC();

	void	registerService(int serviceRequestId,ServiceHandler handler,RequestFactory factory);
	
	int	connect();
	int	disconnect();

	void	sendNotify(DBusNotifyC	*notify);

public:
	int getServerName(char buffer[],int bufferLen);
        int getObjectName(char buffer[],int bufferLen);

private:
	int handleService(int requestId, const QByteArray &requestArg, QByteArray *replyArg);

	friend class DBus_extendAdaptor;

private:
	QDBusConnection		*m_dbusConnection;
	DBus_extendAdaptorImp  	*m_server;
        DBus_extendAdaptor  	*m_serverAdaptor;
	DBusNotifyThreadC      	*m_notifyThread;

	char			*m_serverName;
	char			*m_objectName;

	pthread_mutex_t 	m_lock;

	_ServiceItem		*m_serviceList;
	//char			m_debug;
};

#endif

