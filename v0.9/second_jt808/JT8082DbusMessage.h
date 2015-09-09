#ifndef _JT8082_DBUS_MESSAGE_H_
#define _JT8082_DBUS_MESSAGE_H_

#include <stdio.h>
#include <dbusServer.h>
#include <dbusApplication.h>

#define JT8082_DBUS_SERVER_ID                (char*)"com.innov.jt808s"
#define JT8082_DBUS_SERVER_OBJ               (char*)"/jt808s"



#define JT8082_DBUS_NTF_ID_SETTING_UPDATE            7010

#define JT8082_DBUS_REQ_ID_LOC_REPORT                7020
#define JT8082_DBUS_REP_ID_LOC_REPORT                7021
#define JT8082_DBUS_NTF_ID_LOC_REPORT                7022
#define JT8082_DBUS_NTF_ID_TEMP_LOC_REPORT           7023

#define JT8082_DBUS_NTF_ID_TEXT                      7030

#define JT8082_DBUS_NTF_ID_QUESTION                  7040

#define JT8082_DBUS_REQ_ID_ANSWER                    7050
#define JT8082_DBUS_REP_ID_ANSWER                    7051


#define JT8082_DBUS_NTF_ID_CALL_BACK                 7060

#define JT8082_DBUS_NTF_ID_CONTACTS_UPDATE           7070

#define JT8082_DBUS_NTF_ID_EVENT_SET                 7071
#define JT8082_DBUS_NTF_ID_INFOMATION_DEMAND         7072

#define JT8082_DBUS_NTF_ID_CONN_STATUS               7200
#define JT8082_DBUS_NTF_ID_PARKING_TEST              7400

#define JT8082_DBUS_REQ_ID_DATA_TRANSMIT             7401
#define JT8082_DBUS_NTF_ID_DATA_TRANSMIT             7402



#define JT8082_DBUS_REQ_ID_INFOMATION_DEMAND         7700
#define JT8082_DBUS_REP_ID_INFOMATION_DEMAND         7701

#define JT8082_DBUS_NTF_ID_MULTI_POINT               7096

#define JT8082_DBUS_REQ_ID_COMMON_EVENT              7804
#define JT8082_DBUS_REP_ID_COMMON_EVENT              7805

#define JT8082_DBUS_NTF_ID_NaviElectronicFence     7080
#define JT8082_DBUS_NTF_ID_GNSSAttrChange           7081
#define JT8082_DBUS_NTF_ID_NETBREAK	                     7085

#define JT8082_DBUS_REQ_ID_NaviElectronicFence 7083

#define PHONE_RESET                                     (4)
#define PHONE_RESTORE_DEFAULT                           (5)
#define PHONE_SHORTCUT_NUMBER                           (6)
#define PHONE_PARAS_SET                                 (7)
#define PHONE_ALARM                                     (8)


class JT8082ConnStatusDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082ConnStatusDbusNotifyC)

public:
    JT8082ConnStatusDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_CONN_STATUS)
    {
    }

    void print(FILE *fp)
    {
        fprintf(fp,"\tm_main_status=[%d]\r\n", m_main_status);
        fprintf(fp,"\tm_second_status=[%d]\r\n", m_second_status);
    }
    
    // 0: 断开 1：正在连接 2：已连接
    int m_main_status; //主服务器
    int m_second_status; //备服务器
};
class JT8082ContactsUpdateDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082ContactsUpdateDbusNotifyC)

public:
    JT8082ContactsUpdateDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_CONTACTS_UPDATE) {}
        
    void print(FILE *) {}
};

class JT8082InfomationDbusReqC : public DBusRequestC
{
	DBUS_REQUEST_OBJECT(JT8082InfomationDbusReqC)
	public:
		JT8082InfomationDbusReqC():DBusRequestC(JT8082_DBUS_REQ_ID_INFOMATION_DEMAND){}
		void print(FILE *fp){}

		int sum;
		int flag;
		unsigned char buff[10];

};
class JT8082InfomationDbusRepC:public DBusReplyC
{
	DBUS_REPLY_OBJECT(JT8082InfomationDbusRepC)
	public:
		JT8082InfomationDbusRepC():DBusReplyC(JT8082_DBUS_REP_ID_INFOMATION_DEMAND){}
	       	void print(FILE *) {}
};



class JT8082SettingUpdateDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082SettingUpdateDbusNotifyC)

public:
    JT8082SettingUpdateDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_SETTING_UPDATE) {}
        
    void print(FILE *) {}
    int  flag;
};

class JT8082LocReportDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT8082LocReportDbusReqC)

public:
    JT8082LocReportDbusReqC():DBusRequestC(JT8082_DBUS_REQ_ID_LOC_REPORT){}
    void print(FILE *fp) {fprintf(fp,"\ttableIndex=[%lld]\r\n", m_tableIndex);}
    
    long long m_tableIndex;
};

class JT8082LocReportDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT8082LocReportDbusRepC)

public:
    JT8082LocReportDbusRepC():DBusReplyC(JT8082_DBUS_REP_ID_LOC_REPORT){}
    
    void print(FILE *) {}
};


class JT8082LocReportDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082LocReportDbusNotifyC)

public:
    JT8082LocReportDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_LOC_REPORT) {}
        
    void print(FILE *) {}
    int confirm_alarm;
};

class JT8082TempLocReportDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082TempLocReportDbusNotifyC)

public:
    JT8082TempLocReportDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_TEMP_LOC_REPORT) {}
        
    void print(FILE *fp)
    {
        fprintf(fp,"\tinterval=[%d]\r\n", interval);
        fprintf(fp,"\tvalidity=[%d]\r\n", validity);
    }
    int interval;
    int validity;
};

class JT8082TextDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082TextDbusNotifyC)

public:
    JT8082TextDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_TEXT)
    {
        m_text = NULL;
        DBUS_STRING_MEMBER(m_text);
    }

    ~JT8082TextDbusNotifyC()
    {
        if(m_text)
        {
            free(m_text);
        }
    }
    
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_emergency=[%d]\r\n", m_emergency);
        fprintf(fp,"\tm_lcd=[%d]\r\n", m_lcd);
        fprintf(fp,"\tm_tts=[%d]\r\n", m_tts);
        fprintf(fp,"\tm_bigscreen=[%d]\r\n", m_bigscreen);
        fprintf(fp,"\tm_naviorcan=[%d]\r\n", m_naviorcan);
        if(m_text)
        {
            fprintf(fp,"\tm_text=[%s]\r\n", m_text);
        }
        else
        {
            fprintf(fp,"\tm_text=[NULL]\r\n");
        }
    }
    
    int m_emergency; //1: Emergency
    int m_lcd;       //1: Display on LCD
    int m_tts;       //1: Read by TTS
    int m_bigscreen; //1: Display on BIG screen. ignore now.
    int m_naviorcan; //1: navigation 0:can
    char *m_text;
};

class JT8082InfomationDbusNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(JT8082InfomationDbusNotifyC)

public:
	JT8082InfomationDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_TEXT)
	{
		m_contents = NULL;
		DBUS_STRING_MEMBER(m_contents);
	}   
	~JT8082InfomationDbusNotifyC()
	{   
		if(m_contents)
		{   
			free(m_contents);
		}   

	}   
	void print(FILE *fp)
	{   
		fprintf(fp,"\tm_type=[%d]\r\n", m_type);
		fprintf(fp,"\tm_len=[%d]\r\n", m_len); 
		if(m_contents)
		{   
			fprintf(fp,"\tm_contents=[%s]\r\n", m_contents);
		}
		else
		{
			fprintf(fp,"\tm_contents=[NULL]\r\n");
		}
	}
	int m_type; // type
	int m_len;       //len
	char *m_contents; //content
};

#define JT808_MAX_QUESTION_NUMBER               4

class JT8082QuestionDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082QuestionDbusNotifyC)

public:
    JT8082QuestionDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_QUESTION)
    {
        m_text = NULL;
        m_answer1 = NULL;
        m_answer2 = NULL;
        m_answer3 = NULL;
        m_answer4 = NULL;
        DBUS_STRING_MEMBER(m_text);
        DBUS_STRING_MEMBER(m_answer1);
        DBUS_STRING_MEMBER(m_answer2);
        DBUS_STRING_MEMBER(m_answer3);
        DBUS_STRING_MEMBER(m_answer4);
    }

    ~JT8082QuestionDbusNotifyC()
    {
        if(m_text)
        {
            free(m_text);
        }
        
        if(m_answer1)
        {
            free(m_answer1);
        }

        if(m_answer2)
        {
            free(m_answer2);
        }

        if(m_answer3)
        {
            free(m_answer3);
        }
	
        if(m_answer4)
        {
            free(m_answer4);
        }
    }
    
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_seq=[%d]\r\n", m_seq);
        fprintf(fp,"\tm_emergency=[%d]\r\n", m_emergency);
        fprintf(fp,"\tm_lcd=[%d]\r\n", m_lcd);
        fprintf(fp,"\tm_tts=[%d]\r\n", m_tts);
        fprintf(fp,"\tm_bigscreen=[%d]\r\n", m_bigscreen);
        if(m_text)
        {
            fprintf(fp,"\tm_text=[%s]\r\n", m_text);
        }
        else
        {
            fprintf(fp,"\tm_text=[NULL]\r\n");
        }
        fprintf(fp,"\tm_answerNumber=[%d]\r\n", m_answerNumber);
        fprintf(fp,"\tm_answer1_id=[%d]\r\n", m_answer1_id);
        if(m_answer1)
        {
            fprintf(fp,"\tm_answer1=[%s]\r\n", m_answer1);
        }
        else
        {
            fprintf(fp,"\tm_answer1=[NULL]\r\n");
        }
        fprintf(fp,"\tm_answer2_id=[%d]\r\n", m_answer2_id);
        if(m_answer2)
        {
            fprintf(fp,"\tm_answer2=[%s]\r\n", m_answer2);
        }
        else
        {
            fprintf(fp,"\tm_answer2=[NULL]\r\n");
        }
        fprintf(fp,"\tm_answer3_id=[%d]\r\n", m_answer3_id);
        if(m_answer3)
        {
            fprintf(fp,"\tm_answer3=[%s]\r\n", m_answer3);
        }
        else
        {
            fprintf(fp,"\tm_answer3=[NULL]\r\n");
        }
        fprintf(fp,"\tm_answer4_id=[%d]\r\n", m_answer4_id);
        if(m_answer4)
        {
            fprintf(fp,"\tm_answer4=[%s]\r\n", m_answer4);
        }
        else
        {
            fprintf(fp,"\tm_answer4=[NULL]\r\n");
        }
    }
	int itime;
    int m_seq;       // The sequence of the quesition. should contain it in the answer req message
    int m_emergency; //1: Emergency
    int m_lcd;       //1: Display on LCD
    int m_tts;       //1: Read by TTS
    int m_bigscreen; //1: Display on BIG screen. ignore now.
    char *m_text;
    int m_answerNumber;
    int m_answer1_id;
    char *m_answer1;
    int m_answer2_id;
    char *m_answer2;
    int m_answer3_id;
    char *m_answer3;
    int m_answer4_id;
    char *m_answer4;
};
class JT8082AnswerDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT8082AnswerDbusReqC)

public:
    JT8082AnswerDbusReqC():DBusRequestC(JT8082_DBUS_REQ_ID_ANSWER){}
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_seq=[%d]\r\n", m_seq);
        fprintf(fp,"\tm_answer_id=[%d]\r\n", m_answer_id);
    }
    int time;
    int m_seq;
    int m_answer_id;
};

class JT8082AnswerDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT8082AnswerDbusRepC)

public:
    JT8082AnswerDbusRepC():DBusReplyC(JT8082_DBUS_REP_ID_ANSWER){}
    
    void print(FILE *) {}
};


class JT8082CallBackDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082CallBackDbusNotifyC)

public:
    JT8082CallBackDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_CALL_BACK)
    {
        m_phone = NULL;
        DBUS_STRING_MEMBER(m_phone);
    }

    ~JT8082CallBackDbusNotifyC()
    {
        if(m_phone)
        {
            free(m_phone);
        }
    }
    
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_monitor=[%d]\r\n", m_monitor);
        if(m_phone)
        {
            fprintf(fp,"\tm_phone=[%s]\r\n", m_phone);
        }
        else
        {
            fprintf(fp,"\tm_phone=[NULL]\r\n");
        }
    }
    
    int m_monitor; //1: Monitor, 0: Normal
    char *m_phone;
};
class JT8082InfomationDemandDbusNotifyC : public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082InfomationDemandDbusNotifyC)
public:
    JT8082InfomationDemandDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_INFOMATION_DEMAND){}
    void print(FILE *){}
};
class JT8082EventSetDbusNotifyC : public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082EventSetDbusNotifyC)
public:
    JT8082EventSetDbusNotifyC(): DBusNotifyC(JT8082_DBUS_NTF_ID_EVENT_SET){}
    void print(FILE *){}
};

class JT8082ParkingDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082ParkingDbusNotifyC)

public:
    JT8082ParkingDbusNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_PARKING_TEST) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}
    
    int flag; // 1: 超时停车开始; 2:疲劳检测开始 3:can 4:解除超时停车 5：单次位置 6:更新总里程; 9:车辆控制
    int value; //flag=9 (0:解锁 1: 加锁)
};

class JT8082DataTransmitReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT8082DataTransmitReqC)

public:
    JT8082DataTransmitReqC():DBusRequestC(JT8082_DBUS_REQ_ID_DATA_TRANSMIT){}

    void print(FILE *fp)
    {
        fprintf(fp,"\ttype=[%d]\r\n", type);
    }
    
    int type;       //0x0B 道路运输IC卡信息 0xF0 elec-bill
    int total;
    unsigned char buf[512];
};

class JT8082DataTransmitNotifyC : public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082DataTransmitNotifyC)

public:
    JT8082DataTransmitNotifyC():DBusNotifyC(JT8082_DBUS_NTF_ID_DATA_TRANSMIT){}
    
    void print(FILE *fp)
    {
        fprintf(fp,"\ttype=[%d]\r\n", type);
    }
    int type;
    int errnum;
    int total;
    unsigned char buf[512];
};
#if 0    //zanbuchuli 
class JT8082MultiPointDbusNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(JT808MultiPointDbusNotifyC)

	public:
		JT8082MultiPointDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_MULTI_POINT)
	{   
	}   

		void print(FILE *)
		{   
			;   
		}   

		int m_result;
};

class JT8082LocTimeDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808LocTimeDbusNotifyC)

public:
    JT8082LocTimeDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_LOCTIME){}
	
	unsigned int LocUtcTime;
	unsigned int LocInterval;
};
#endif
class JT8082CommonEventReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT8082CommonEventReqC)

public:
    JT8082CommonEventReqC():DBusRequestC(JT8082_DBUS_REQ_ID_COMMON_EVENT){}
    
    int type;   // 1:恢复出厂设置2 sim 修改 3  第二服务器修改
    int value;
};
class JT8082CommonEventRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT8082CommonEventRepC)

public:
    JT8082CommonEventRepC():DBusReplyC(JT8082_DBUS_REP_ID_COMMON_EVENT){}
    
   void print(FILE *fp)
    {
        fprintf(fp,"\tm_result=[%d]\r\n", m_result);
    }
    int m_result;
};


class JT8082NaviElectronicFenceReqC :public DBusRequestC
{
DBUS_REQUEST_OBJECT(JT8082NaviElectronicFenceReqC)
public:

   JT8082NaviElectronicFenceReqC():DBusRequestC(JT8082_DBUS_REQ_ID_NaviElectronicFence){} 
};
class JT8082NaviElectronicFenceNotifyC : public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT8082NaviElectronicFenceNotifyC)
    public:
		JT8082NaviElectronicFenceNotifyC() : DBusNotifyC(JT8082_DBUS_NTF_ID_NaviElectronicFence){}

		int eflag;        //(estate==0有效)操作类型 1--增加 0--修改  -1--删除
       int eid;           //(estate==0有效)电子围栏信息在数据库中的存储id
       int etype;       //电子围栏的类型  0--圆形  1--矩形  2--多边形  3--线路
       int estate;
};

class JT8082GNSSAttrChangeNotifyC : public DBusNotifyC
{
       DBUS_NOTIFY_OBJECT(JT8082GNSSAttrChangeNotifyC)

 public:
 JT8082GNSSAttrChangeNotifyC():DBusNotifyC(JT8082_DBUS_NTF_ID_GNSSAttrChange){}
	
   int mode;
   int baudrate;      
   int outfrequency;
   int samplerate;
	int gnss_flag;
   int refresh;
   int reconnect;
   int reregister;
};

class JT8082NetBreakNotifyC : public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(JT8082NetBreakNotifyC)
public:
	JT8082NetBreakNotifyC():DBusNotifyC(JT8082_DBUS_NTF_ID_NETBREAK){}
	
};



#endif
