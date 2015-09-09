#ifndef _DEVICE_MANAGER_DBUS_MESSAGE_H_
#define _DEVICE_MANAGER_DBUS_MESSAGE_H_

#include <stdio.h>

#include "dbusMessage.h"

#define DEVICE_MANAGER_DBUS_SERVER_ID              (char*)"com.innov.devicemanager"
#define DEVICE_MANAGER_DBUS_SERVER_OBJ             (char*)"/devicemanager"

#define DEVICE_MANAGER_NOTF_ID_DRIVE_LOGIN          5600
#define DEVICE_MANAGER_NOTF_ID_FENCE_INOUT          5601
#define DEVICE_MANAGER_NOTF_ID_ACCOFF               5602    //关机通知
#define DEVICE_MANAGER_NOTF_ID_REVERSE              5603    //倒车
#define DEVICE_MANAGER_NOTF_ID_SPEED_ABNORMAL       5604    //速度状态异常
#define DEVICE_MANAGER_NOTF_ID_TIRED                5605    //超时驾驶
#define DEVICE_MANAGER_NOTF_ID_ALARM                5606    // 报警
#define DEVICE_MANAGER_NOTF_ID_CAMERA               5607    //拍照
#define DEVICE_MANAGER_NOTF_ID_RUN_STOP             5608    //运行状态 停止状态
#define DEVICE_MANAGER_NOTF_ID_DB9_CHECk            5609    //DB9 check 
#define DEVICE_MANAGER_NOTF_ID_UPDATE_MILEAGE       5610    //重新读取初始里程
#define DEVICE_MANAGER_NOTF_ID_SMS_ALARM_INFO       5611    //报警发送文本SMS
#define DEVICE_MANAGER_NOTF_ID_ACC_INFO             5612    //acc 状态on off 通知
#define DEVICE_MANAGER_NOTF_ID_DRIVING_STATE        5613    //行车状态通知 公交项目
#define DEVICE_MANAGER_NOTF_ID_INSGPS_INFO          5614    //惯导车位信息
#define DEVICE_MANAGER_NOTF_ID_COMMON_EVENT         5615    //通用通知

#define REQ_ID_REBOOT_MACHINE                       5700
#define REQ_ID_SHUTDOWN_MACHINE                     5701
#define REQ_ID_CHECK_TIME                           5702
#define REQ_ID_CARRY_STATUS                         5703

#define REQ_ID_DATA_UP_PASSTHROUGH                  5704    //数据上行透传
#define REQ_ID_SPEED_RATIO_UPDATE                   5705
#define REQ_ID_GPS_SPEED_REPLACE_VDR_SPEED          5706    //gps speed 替代 vdr speed
#define REQ_ID_WIRELESS_INTENSITY                   5707    //无线信号强度
#define REQ_ID_GUI_EMERGENCY_ALARM                  5708    //gui 触发紧急报警请求
#define REQ_ID_DEV_COMMON_EVENT                     5709    //通用请求
#define REQ_ID_INSGPS_INFO_ANSWER                   5710    //惯导车位信息应答请求


class DeviceDriveLoginDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceDriveLoginDbusNotifyC)

public:
    DeviceDriveLoginDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_DRIVE_LOGIN) {}
        
    void print(FILE *fp) {fprintf(fp,"\tdriverid=%s\r\n",driverid);}

    int command; // 0x40~0x43 道路运输IC卡  0x33 渣土车IC卡
    int type; //    command=0x33 时  type=1 有权限卡, type=0 无权限卡
    char driverid[20]; 
};

class DeviceFenceInOutDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceFenceInOutDbusNotifyC)

public:
    DeviceFenceInOutDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_FENCE_INOUT) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;     // 1:进入区域  0:退出区域 2: UI不提示3:渣土车进入限速区域 4:渣土车进入举升区域
    int cm_flag;  // 1:开启通信 0:关闭通信  flag=3时表示限速值  flag=4时1 举升开 0举升关
    int gnss_flag; // 1:采集GNSS,  0:不采集GNSS
};

class DeviceAccOffDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceAccOffDbusNotifyC)

public:
    DeviceAccOffDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_ACCOFF) {}
        
    void print(FILE *) {}
    int flag; // 0 normal , 1 remotectrl shutdown , 2 reboot
};

class DeviceReverseDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceReverseDbusNotifyC)

public:
    DeviceReverseDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_REVERSE) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d,icount=%d\r\n",flag,icount);}

    int flag;  // 1 reverse, 0  cancel, 2 left , 3 right
    int icount;
};

class DeviceSpeedAbnormalDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceSpeedAbnormalDbusNotifyC)

public:
    DeviceSpeedAbnormalDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_SPEED_ABNORMAL) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}
    int flag;   //0:正常，1:异常
};

class DeviceTiredDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceTiredDbusNotifyC)

public:
    DeviceTiredDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_TIRED),flag(0),dura(0) {
	    memset(driverid,0,18);
    }
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;   // 0 :(tired_time-30)至tired_time 1: tired_time至tired_time+30 2:大于tired_time+30 3:解除疲劳驾驶 (tired_time为疲劳驾驶时间门限)
    int dura;   // 连续驾驶时间
    char driverid[18];
};

typedef enum
{
    DEV_ALARM_EMERGENCY_ALARM = 0,      // 0: 紧急报警
    DEV_ALARM_SMOKE_ALARM,              // 1:  烟雾报警
    DEV_ALARM_COLLISION_ALARM,          // 2: 碰撞
    DEV_ALARM_PARKING_OVERTIIME,        // 3: 超时停车
    DEV_ALARM_POWER_UNDER_VOLTAGE,       // 4: 主电源欠压
    DEV_ALARM_TURNOVER_ALARM                // 5: 侧翻
}ENUM_DEV_ALARM_COL;

class DeviceAlarmDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceAlarmDbusNotifyC)

public:
    DeviceAlarmDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_ALARM) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int type;       
    int flag;       //  1:  报警  0 : 解除报警
};

class DeviceCameraDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceCameraDbusNotifyC)

public:
    DeviceCameraDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_CAMERA){}

    void print(FILE *fp)
    {
        fprintf(fp,"\tm_channel=[%d]\r\n", m_channel);
        fprintf(fp,"\tm_command=[%d]\r\n", m_command);
        fprintf(fp,"\tm_interval=[%d]\r\n", m_interval);
        fprintf(fp,"\tm_flag=[%d]\r\n", m_flag);
        fprintf(fp,"\tm_resolution=[%d]\r\n", m_resolution);
        fprintf(fp,"\tm_event=[%d]\r\n", m_event);
    }
    
    int m_channel;      // 通道开关标志 位操作  0 :不允许  1:允许
    int m_command;      //拍摄命令     0:停止拍照   0xFFFF:录像     n:  Number of Pictures
    int m_interval;     //拍照间隔 n  0:最小时间间隔
    int m_flag;         // 保存标志   位操作      0: 存储   1: 上传 
    int m_resolution;   // 分辨率        0x02: 640*480
    int m_event;       
                        //事件编码   0:平台下发   1:定时 2:抢劫报警 3:碰撞侧翻
                        //                        4:门开 5:门关 6: 门开关20<speed  7:定距8:保留
};

class DeviceRunStopDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceRunStopDbusNotifyC)

public:
    DeviceRunStopDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_RUN_STOP) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d， itime=%d\r\n",flag,itime);}

    int flag;       //  0:运行状态  1 : 停运状态
    int itime;      //  发生时间
};

class DeviceDB9CheckDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceDB9CheckDbusNotifyC)

public:
    DeviceDB9CheckDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_DB9_CHECk) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;       //  0:退出检定状态  1 : 进入检定状态
};

class DeviceUpdateMileageNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceUpdateMileageNotifyC)

public:
    DeviceUpdateMileageNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_UPDATE_MILEAGE) {}
    
    int mileage;// 最新的初始里程 单位: 0.1km
};

class DeviceSMSAlarmNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceSMSAlarmNotifyC)

public:
    DeviceSMSAlarmNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_SMS_ALARM_INFO) {}
    
    int s_alarm; //报警位
    int s_itime; //报警时间
    int s_fx;
    int s_fy;
};

class DeviceACCInfoDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceACCInfoDbusNotifyC)

public:
    DeviceACCInfoDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_ACC_INFO) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;       //  0: acc off;  1: acc on 
};


class RebootMachineReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(RebootMachineReqC) 

public: 
    RebootMachineReqC():DBusRequestC(REQ_ID_REBOOT_MACHINE){} 
};

class ShutdownMachineReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(ShutdownMachineReqC) 

public: 
    ShutdownMachineReqC():DBusRequestC(REQ_ID_SHUTDOWN_MACHINE){} 
};

class CheckTimeReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(CheckTimeReqC) 

public: 
    CheckTimeReqC():DBusRequestC(REQ_ID_CHECK_TIME){} 
};

class CarryStatusReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(CarryStatusReqC) 

public: 
    CarryStatusReqC():DBusRequestC(REQ_ID_CARRY_STATUS){} 
    int carry_status;//0:空车   1:半载    2:保留    3:满载
};

class DeviceDataUpPassReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(DeviceDataUpPassReqC) 

public: 
    DeviceDataUpPassReqC():DBusRequestC(REQ_ID_DATA_UP_PASSTHROUGH){} 
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;  // 1: 数据上行透传
};

class SpeedRatioUpdateReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(SpeedRatioUpdateReqC) 

public: 
    SpeedRatioUpdateReqC():DBusRequestC(REQ_ID_SPEED_RATIO_UPDATE){} 
    int speed_factor;
    int lap_factor;
};

class GPSReplaceVDRSpeedReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(GPSReplaceVDRSpeedReqC) 

public: 
    GPSReplaceVDRSpeedReqC():DBusRequestC(REQ_ID_GPS_SPEED_REPLACE_VDR_SPEED){} 
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;  //  1:  gps speed replace vdr speed;  0: cancel;
};

class WirelessIntensityReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(WirelessIntensityReqC) 

public: 
    WirelessIntensityReqC():DBusRequestC(REQ_ID_WIRELESS_INTENSITY){} 
    void print(FILE *fp) {fprintf(fp,"\tsignal_strength=%d\r\n",signal_strength);}

    int signal_strength;  
};

class EmergencyAlarmReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(EmergencyAlarmReqC) 

public: 
    EmergencyAlarmReqC():DBusRequestC(REQ_ID_GUI_EMERGENCY_ALARM){} 
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag;  //  1:紧急报警;
};


class DevCommonEventNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DevCommonEventNotifyC)

public:
    DevCommonEventNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_COMMON_EVENT) {}
        
    void print(FILE *fp) {fprintf(fp,"\ttype=%d\r\n",type);}

    int type;   //   1 激活;  2 解锁;
    int value;  // 暂时无用
};


class DevCommonEventReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(DevCommonEventReqC) 

public: 
    DevCommonEventReqC():DBusRequestC(REQ_ID_DEV_COMMON_EVENT){} 
    void print(FILE *fp) {fprintf(fp,"\t type=%d,value=%d,flag=%d\r\n",type,value,flag);}

    int type;  // 1: gnss mode;  2:左右转  3:初始里程
    int value; // 1:(1gps 2bd 3gn) ;  2:(0:off,1:on); 3:(单位0.1km 0~9999999)
    int flag;
};

class DevINSGPSInfoDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DevINSGPSInfoDbusNotifyC)

public:
    DevINSGPSInfoDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_INSGPS_INFO) {}
        
    void print(FILE *fp) {fprintf(fp,"\ttype=%d\r\n",type);}

    int type; //0x60: 分配车位号; 0x59: 分配车位和实际车位
    unsigned char distribute[8];
    unsigned char actual[8];
};

class DevINSGPSAnswerReqC: public DBusRequestC 
{ 
    DBUS_REQUEST_OBJECT(DevINSGPSAnswerReqC) 

public: 
    DevINSGPSAnswerReqC():DBusRequestC(REQ_ID_INSGPS_INFO_ANSWER){} 
    void print(FILE *fp) {fprintf(fp,"\t type=%d,value=%d,\r\n",type,value);}

    int type;  // 0x60   0x59  0x57
    int value; // 1:确定 2:取消
    unsigned char actual[8];
};

#define DEVICE_MANAGER_DBUS_DRIVING_STATE_NORMAL  (0x01 << 3)
#define DEVICE_MANAGER_DBUS_DRIVING_STATE_LEFT    (0x01 << 4)
#define DEVICE_MANAGER_DBUS_DRIVING_STATE_RIGHT   (0x01 << 5)
#define DEVICE_MANAGER_DBUS_DRIVING_STATE_BRAKE   (0x01 << 6)
#define DEVICE_MANAGER_DBUS_DRIVING_STATE_SOS     (0x01 << 10)


class DeviceDrivingStateDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(DeviceDrivingStateDbusNotifyC)

public:
    DeviceDrivingStateDbusNotifyC() : DBusNotifyC(DEVICE_MANAGER_NOTF_ID_DRIVING_STATE) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}

    int flag; //  3:正常行驶 4:左转弯 5:右转弯6:刹车10:SOS    
};

#endif
