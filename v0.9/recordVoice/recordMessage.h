#ifndef RECORDMESSAGE_H
#define RECORDMESSAGE_H

/** *********************************************************
    说明:
    此文件为与v0.9库的recordvoice的recordMessage.h公用的文件
    GuiRecordNotifyC 成员变量说明：
    command: 1:开始 0:停止
    flag: 对应 clientMain.h中宏定义的 m_flag
        UploadRecord : 会将录音记录到数据库
        SaveRecord,  : 只是保存在 nand中
        TestRecord   : GUI测试功能，或保存在nand的test.wav文件
    sample: 0 1 2 3
    time: 录音时间长度，最长为10分钟 10*60=600，在GUI测试时应指定
    为0，以便调用停止录音能及时生效，否则会阻塞，一直录到指定时间长度

    [zhenyubin 2014/12/30]
************************************************************/

#include "dbusMessage.h"

#define NOTF_ID_GUI_RECORD  1177
#define REQ_ID_GUI_RECORD   1177
#define REP_ID_GUI_RECORD   1177

//录音通知,消息,回复 [zhenyubin 2014/12/30]
enum enuRecordCommand
{
    StopRecord=0,
    StartRecord
};

/** ***********************************************************
    存储标志
    对应 clientMain.h中宏定义
    的 m_flag
    #define UPLOAD 0
    #define SAVE 1
    #define TEST 2 //Gui录音测试功能 [zhenyubin 2014/12/30]
**************************************************************/
enum enuRecordFlag
{
    UploadRecord =0,
    SaveRecord,
    TestRecord
};
class GuiRecordNotifyC:public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(GuiRecordNotifyC)

public:
    GuiRecordNotifyC():DBusNotifyC(NOTF_ID_GUI_RECORD){}
    ~GuiRecordNotifyC(){}

    enuRecordCommand command; //1: 开始 0：停止
    enuRecordFlag flag; //保存还是测试
    int sample; //采样率 0: 8000 1: 11000; 2: 23000; 3: 32000;
    int time; //录音时长 Gui测试应该指定为0 ，才能及时启用停止命令
};
class GuiReqRecordC:public DBusRequestC
{
    DBUS_REQUEST_OBJECT(GuiReqRecordC)

public:
    GuiReqRecordC():DBusRequestC(REQ_ID_GUI_RECORD){}
    char filePath[256]; //录音文件名
};
class GuiRepRecordC:public DBusReplyC
{
    DBUS_REPLY_OBJECT(GuiRepRecordC)

public:
    GuiRepRecordC():DBusReplyC(REP_ID_GUI_RECORD){}
    bool received;
};

#endif // RECORDMESSAGE_H
