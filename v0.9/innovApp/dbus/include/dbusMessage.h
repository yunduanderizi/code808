#ifndef DBUS_MESSAGE_H_20100617
#define DBUS_MESSAGE_H_20100617

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef void (*DBusMessageFreeFunc)(void *buffer);

void DBusMessageFree(void *buffer);
void DBusMessageNoFree(void *buffer);

#define DBUS_MESSAGE_OBJECT(MessageClassName,MessageClassType) \
	public: \
		virtual int encode(void *&buffer,int &len,DBusMessageFreeFunc &freeFunc) \
		{\
			len=sizeof(MessageClassName)-sizeof(DBusMessageC); \
			buffer=(void*)(((unsigned char*)this)+sizeof(DBusMessageC));\
			freeFunc=NULL;\
			if(m_stringMemberCount>0)\
			{\
				int  tempLen =len;\
				int  stringMemberLen[DBUS_MAX_STRING_MEMBERS];\
				for(int i=0;i<m_stringMemberCount;i++)\
				{\
					if(*(m_stringMemberAddr[i]) !=NULL)\
					{\
						stringMemberLen[i] =strlen(*(m_stringMemberAddr[i]))+1;\
						tempLen +=stringMemberLen[i];\
					}\
				}\
				void *tempBuffer =malloc(tempLen);\
				bcopy(buffer,tempBuffer,len);\
				for(int i=0;i<m_stringMemberCount;i++)\
				{\
					if(*(m_stringMemberAddr[i]) !=NULL)\
					{\
						strcpy((char*)tempBuffer+len,*(m_stringMemberAddr[i]));\
						len +=stringMemberLen[i];\
						bcopy((char*)&stringMemberLen[i],(char*)tempBuffer+((char*)(m_stringMemberAddr[i])-(char*)this-sizeof(DBusMessageC)),sizeof(stringMemberLen[i]));\
					}\
				}\
				buffer   =tempBuffer;\
				freeFunc =&free;\
			}\
			return 0; \
		}\
		virtual int decode(void *buffer,int len)\
		{\
			int  thisLen =(int)(sizeof(MessageClassName)-sizeof(DBusMessageC));\
			int  tempLen =thisLen;\
			if(len<thisLen)\
				return -1;\
			int  stringMemberLen;\
			char* stringMemberValue[DBUS_MAX_STRING_MEMBERS];\
			for(int i=0;i<m_stringMemberCount;i++)\
			{\
				bcopy((char*)buffer+((char*)(m_stringMemberAddr[i])-(char*)this-sizeof(DBusMessageC)),(char*)&stringMemberLen,sizeof(stringMemberLen));\
				if(stringMemberLen>0)\
				{\
					if(tempLen+stringMemberLen > len || (int)(strlen((char*)buffer+tempLen)+1) !=stringMemberLen)\
					{\
						for(int j=0;j<i;j++)\
							if(stringMemberValue[j])\
								free(stringMemberValue[j]);\
						return -2;\
					}\
					stringMemberValue[i] =strdup((char*)buffer+tempLen);\
					tempLen +=stringMemberLen;\
				}\
				else\
				{\
					stringMemberValue[i] =NULL;\
				}\
			}\
			if(tempLen ==0)\
				return 0;\
			void* p=(void*)(((unsigned char*)this)+sizeof(DBusMessageC));\
			bcopy(buffer,p,thisLen);\
			for(int i=0;i<m_stringMemberCount;i++)\
			{\
				*(m_stringMemberAddr[i]) =stringMemberValue[i]; \
			}\
			return 0;\
		}\
		static MessageClassType* factory()\
		{\
			return new MessageClassName();\
		}\
		private:


#define DBUS_REQUEST_OBJECT(RequestClassName) 	DBUS_MESSAGE_OBJECT(RequestClassName,DBusRequestC) 
#define DBUS_REPLY_OBJECT(ReplyClassName) 	DBUS_MESSAGE_OBJECT(ReplyClassName,DBusReplyC) 
#define DBUS_NOTIFY_OBJECT(NotifyClassName) 	DBUS_MESSAGE_OBJECT(NotifyClassName,DBusNotifyC) 

#define DBUS_MAX_STRING_MEMBERS 		6
#define DBUS_STRING_MEMBER(StringMemberName)	do{if(m_stringMemberCount<DBUS_MAX_STRING_MEMBERS) \
					{m_stringMemberAddr[m_stringMemberCount++]=(char**)(&(StringMemberName)); StringMemberName=NULL;}} while(0);



class DBusMessageC
{
public:
	DBusMessageC(int id);
	virtual ~DBusMessageC();

	virtual int encode(void *&buffer,int  &len,DBusMessageFreeFunc &freeFunc);
	virtual int decode(void *buffer,int len);
	virtual void print(FILE *fp);

	int	getIdentifier();

public:
	void setArgBuffer(void *buffer,int len,DBusMessageFreeFunc freeFunc);

protected:
	void printBuffer(FILE *fp,void* buffer,int len);

protected:
	unsigned char	m_stringMemberCount;
	char		**m_stringMemberAddr[DBUS_MAX_STRING_MEMBERS];

private:
	int 	     m_identifier;

	int 	     m_argLen;
	void	     *m_argBuffer;
	DBusMessageFreeFunc m_freeFunc;

	friend class DBusReplyC;
};

class DBusRequestC: public DBusMessageC
{
public:
	DBusRequestC(int id):DBusMessageC(id){}
	virtual ~DBusRequestC() {}

	virtual void print(FILE *fp);

public:
	static DBusRequestC* factory();
};


class DBusReplyC: public DBusMessageC
{
public:
	DBusReplyC(int id):DBusMessageC(id){}
	virtual ~DBusReplyC(){}

	virtual void print(FILE *fp);

public:
	int	result;

public:
	static DBusReplyC* factory();
};

class DBusNotifyC: public DBusMessageC
{
public:
	DBusNotifyC(int id):DBusMessageC(id){}
	virtual ~DBusNotifyC(){}

	virtual void print(FILE *fp);

public:
	static DBusNotifyC* factory();
};


#define DBUS_REP_ID_COMMON	0

#define DBUS_REP_RESULT_SUCCESS		0	
#define DBUS_REP_RESULT_NO_SERVICE	-1
#define DBUS_REP_RESULT_DECODE_ERROR	-2
#define DBUS_REP_RESULT_ENCODE_ERROR	-3
#define DBUS_REP_RESULT_PROCESS_ERROR	-4
#define DBUS_REP_RESULT_NOT_VALID	-100

class DBusCommonReplyC: public DBusReplyC
{
	DBUS_REPLY_OBJECT(DBusCommonReplyC)

public:
	DBusCommonReplyC():DBusReplyC(DBUS_REP_ID_COMMON)
	{ 
		result=DBUS_REP_RESULT_NOT_VALID; 
	}
	DBusCommonReplyC(int _result):DBusReplyC(DBUS_REP_ID_COMMON) {result=_result;}
};



class abc:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(abc) 
public:
	abc():DBusRequestC(10)
	{
		i=1098;
	}

public:

	int i;
	int f;
	char buffer[100];	
};

class abc1:public DBusReplyC\
{
	DBUS_REPLY_OBJECT(abc1) 
public:
	abc1():DBusReplyC(11)
	{
	}

};
class abc2:public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(abc2) 
public:
	abc2():DBusNotifyC(12)
	{
	}
};



#endif

