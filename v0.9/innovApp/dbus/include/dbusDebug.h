
#ifndef DBUS_DEBUG_H_20100926
#define	DBUS_DEBUG_H_20100926 

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>

#include "dbusMessage.h"

class DBusDebugC
{
public:
	static void init(const char *processName);
	static void openDebug(FILE *fp=stdout);
	static void closeDebug();

private:
	static inline void print(char const *fmt,...)
	{
		if(m_processName==NULL)
		{
			if(getenv("INNOV_DEBUG")==NULL)
				return;

			va_list ap;
			va_start(ap,fmt);

			fprintf(stdout,"%4d. ",m_serial++);
			vfprintf(stdout,fmt,ap);

			va_end(ap);	
		}
		else
		{
			pthread_mutex_lock(&m_lock);
			if(m_fp==NULL)
			{
				pthread_mutex_unlock(&m_lock);
				return;
			}

			va_list ap;
			va_start(ap,fmt);

			fprintf(m_fp,"%4d. %s ",m_serial++,m_processName);
			vfprintf(m_fp,fmt,ap);

			va_end(ap);	
			pthread_mutex_unlock(&m_lock);
		}
	}

	static inline void print(DBusRequestC *msg,char *serverName,char isServer)
	{
		if(m_processName==NULL)
		{
			if(getenv("INNOV_DEBUG")==NULL)
				return;

			if(isServer)
			{
				fprintf(stdout,"%4d. Server=%s: Received. requestId=%d\n",m_serial++,serverName,msg->getIdentifier());
			}
			else
			{
				fprintf(stdout,"%4d. Client=%s: Send. requestId=%d\n",m_serial++,serverName,msg->getIdentifier());
			}

			msg->print(stdout);
		}
		else
		{
			pthread_mutex_lock(&m_lock);
			if(m_fp==NULL)
			{
				pthread_mutex_unlock(&m_lock);
				return;
			}

			if(isServer)
			{
				fprintf(m_fp,"%4d. %s Server=%s: Received. requestId=%d\n",m_serial++,m_processName,serverName,msg->getIdentifier());
			}
			else
			{
				fprintf(m_fp,"%4d. %s Client=%s: Send. requestId=%d\n",m_serial++,m_processName,serverName,msg->getIdentifier());
			}

			msg->print(m_fp);

			pthread_mutex_unlock(&m_lock);
		}
	}
	static inline void print(DBusReplyC *msg,char *serverName,char isServer)
	{
		if(m_processName==NULL)
		{
			if(getenv("INNOV_DEBUG")==NULL)
				return;

			if(isServer)
			{
				fprintf(stdout,"%4d. Server=%s: Send. replyId=%d result=%d\n",m_serial++,serverName,msg->getIdentifier(),msg->result);
			}
			else
			{
				fprintf(stdout,"%4d. Client=%s: Received. replyId=%d result=%d\n",m_serial++,serverName,msg->getIdentifier(),msg->result);
			}

			msg->print(stdout);
		}
		else
		{
			pthread_mutex_lock(&m_lock);
			if(m_fp==NULL)
			{
				pthread_mutex_unlock(&m_lock);
				return;
			}

			if(isServer)
			{
				fprintf(m_fp,"%4d. %s Server=%s: Send. replyId=%d result=%d\n",m_serial++,m_processName,serverName,msg->getIdentifier(),msg->result);
			}
			else
			{
				fprintf(m_fp,"%4d. %s Client=%s: Received. replyId=%d result=%d\n",m_serial++,m_processName,serverName,msg->getIdentifier(),msg->result);
			}

			msg->print(m_fp);

			pthread_mutex_unlock(&m_lock);
		}
	}
	static inline void print(DBusNotifyC *msg,char *serverName,char isServer)
	{
		if(m_processName==NULL)
		{
			if(getenv("INNOV_DEBUG")==NULL)
				return;

			if(isServer)
			{
				fprintf(stdout,"%4d. Server=%s:  Send. notifyId=%d\n",m_serial++,serverName,msg->getIdentifier());
			}
			else
			{
				fprintf(stdout,"%4d. Client=%s: Received. notifyId=%d\n",m_serial++,serverName,msg->getIdentifier());
			}

			msg->print(stdout);
		}
		else
		{
			pthread_mutex_lock(&m_lock);
			if(m_fp==NULL)
			{
				pthread_mutex_unlock(&m_lock);
				return;
			}

			if(isServer)
			{
				fprintf(m_fp,"%4d. %s Server=%s:  Send. notifyId=%d\n",m_serial++,m_processName,serverName,msg->getIdentifier());
			}
			else
			{
				fprintf(m_fp,"%4d. %s Client=%s: Received. notifyId=%d\n",m_serial++,m_processName,serverName,msg->getIdentifier());
			}

			msg->print(m_fp);

			pthread_mutex_unlock(&m_lock);
		}
	}

private:
	static pthread_mutex_t  m_lock;
	static FILE		*m_fp;
	static unsigned int	m_serial;
	
	static char		*m_processName;

	friend class DBusServerC;
	friend class DBusNotifyThreadC;
	friend class DBusClientC;
};


#endif

