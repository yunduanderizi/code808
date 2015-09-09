#ifndef DBUS_CLIENT_H_20100621
#define DBUS_CLIENT_H_20100621

#include <pthread.h>

#include "dbusMessage.h"

class QByteArray;
class QDBusConnection;
class DBus_extendProxy;
struct _NotifyItem;
struct _ReplyItem;


#define DBUS_RESULT_SUCCESS		0
#define DBUS_RESULT_ERROR_DISCONNECT	-1
#define DBUS_RESULT_ERROR_TIMEOUT	-2
#define DBUS_RESULT_ERROR_ENCODE	-3
#define DBUS_RESULT_ERROR_DECODE	-4
#define DBUS_RESULT_ERROR_PARAM		-5
#define DBUS_RESULT_ERROR_REPLY_NOT_RECOGNIZED		-6
#define DBUS_RESULT_ERROR_NO_SERVER	-7
#define DBUS_RESULT_ERROR_UNKNOWN	-100


typedef DBusReplyC*   (*ReplyFactory)(void);
typedef DBusNotifyC*   (*NotifyFactory)(void);
typedef void (*NotifyHandler)(DBusNotifyC *notify);

class DBusClientC
{
public:
	DBusClientC(char *serverName,char *objectName);
	~DBusClientC();

	void	registerReply(int serviceReplyId,ReplyFactory factory);
	void	registerNotify(int serviceNotifyId,NotifyHandler handler,NotifyFactory factory);
	
	int	connect();
	int	disconnect();

	int sendService(DBusRequestC  *request);
	inline int sendService(DBusRequestC  *request,DBusReplyC *&reply,int timeout) //timeout unit is millisecond
	{
		return sendServiceInternal(request,reply,timeout,0);
	}
	inline int sendServiceSubThread(DBusRequestC  *request,DBusReplyC *&reply,int timeout) //timeout unit is millisecond
	{
		return sendServiceInternal(request,reply,timeout,1);
	}

public:
	int getServerName(char buffer[],int bufferLen);
	int getObjectName(char buffer[],int bufferLen);

private:
	int sendServiceInternal(DBusRequestC  *request,DBusReplyC *&reply,int timeout,int blockFlag); //timeout unit is millisecond
	void handleNotify(int notifyId, const QByteArray &notifyArg);

	friend class DBus_extendProxy;

private:
	QDBusConnection		*m_dbusConnection;
        DBus_extendProxy	*m_serverProxy;

	char			*m_serverName;
	char			*m_objectName;

	pthread_mutex_t 	m_lock;

	_NotifyItem		*m_notifyList;
	_ReplyItem		*m_replyList;

	//char			m_debug;
};

#endif

