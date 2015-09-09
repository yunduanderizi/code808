#include "hmi/map_mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QResource>
#include <QPixmapCache>
#include "gps/satellitemodule.h"
#include "fontset.h"
#include "hmi/map_guide.h"
#include "hmi/guidethread.h"
#include "engineinterface/interface_route.h"
#if defined(ARM_LINUX)
#include "fence/fencemanager.h"
#include "dbus/dbusinterface.h"
#include "inputmethod/InputInit.h"
#include "inputmethod/InputMethod.h"

#include "TTS/read_news.h"
#include "alsa/asoundlib.h"
#include "TTS/audiocontrolcenter/include/audio_control.h"
#include "TTS/audiocontrolcenter/include/audio_control_client.h"
#include "TTS/audiocontrolcenter/include/audio_control_server.h"
#include "TTS/audiocontrolcenter/include/audio_main_volume.h"

#include <QStringList>
extern DBusClientProxyC *g_pAudioClient;
class GPSHandleC:public AudioCtrlCBHandlerC
{
public:
    void notifyHandle(int, int, unsigned char){}
};

WindowManagerApp* g_pWindowManager;
FenceManager *s_pfencemanager = new FenceManager();
#endif

QMainWindow *g_mainwindow = NULL;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont s_font(FONT_FAMILY);
    s_font.setPointSize(STAND_FONT_SIZE);
    a.setFont(s_font);
    QTextCodec *codec = QTextCodec::codecForName("gb18030");
    QTextCodec *OldTextCodec = QTextCodec::codecForTr();
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec); //test
    bool bRetFlag = false;
    bRetFlag = QResource::registerResource("cast.rcc");
    if (!bRetFlag)
    {
        printf("QResource::registerResource fail!\n");
    }
    QPixmapCache::setCacheLimit(10240);
    routeDefaultOptions();

#if defined(ARM_LINUX)


    DBusDebugC::init("testServer");
    YT_inputInit();
    unsigned int version =DBusApplicationC::version();
            printf("DBUS version: %d.%d.%d     Release date: %s\n",version>>24,(version>>16)&0xFF,(version & 0xFFFF),DBusApplicationC::releaseDate());

    DBusServerC	server((char*)"com.YunTu.Navi",(char*)"/Navi");

    g_pServer	=&server;
    if(NULL != g_pServer)
    {
        g_pServer->registerService(REQ_ID_NAVI_CARLOCATION,CarLocationHandler,CNaviCarLocationReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_SHOWMENU,ShowMenuHandler,CNaviShowMenuC::factory);
        g_pServer->registerService(REQ_ID_NAVI_GPSINFO_REPORT_CYCLE,GPSInfoReportCycleHandler,CNaviGPSInfoReportCycleReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_ROADBOOK,RoadBookHandler,CNaviRoadBookReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_ROADBOOK2,RoadBook2Handler,CNaviRoadBook2ReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_HOSPITAL,HospitalHandler,CNaviHospitalReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_GPSINFO,GPSInfoHandler,CNaviGPSInfoReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_WINDOWCONTROL,WindowDisplayControlInfoHandler,CNaviWindowDisplayControlReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_GASSTATION,GasStationHandler,CNaviGasStationReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_PARK,ParkHandler,CNaviParkReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_LOCATECAR,LocateCarHandler,CNaviLocateCarReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_LOCATEHOME,LocateHomeHandler,CNaviLocateHomeReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_DESTINATON,DestinatonHandler,CNaviDestinatonReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_DESTINATONRECORD,DestinatonRecordHandler,CNaviDestinatonRecordReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_NAVIGATION,NavigationHandler,CNaviNavigationReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_REALTIMEROAD,RealTimeRoadHandler,CNaviRealTimeRoadReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_ROADREPORT,RoadReportHandler,CNaviRoadReportReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_COMMONREQ,CommonReqHandler,CNaviCommonReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_ADDRESSBOOK,AddressBookHandler,CNaviAddressBookReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_SURROUNDINGSEARCH,SurroundingSearchHandler,CNaviSurroundingSearchReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_DAYNIGHTMODE,DayNightModeHandler,CNavDayNightModeReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_RIGHT_BUTTON_ROTATE,RightButtonRotateHandler,CNavRightButtonRotateReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_MULTIPOINTGPSINFO,MultiPointHandler,CNaviMultipointReqC::factory);
        g_pServer->registerService(REQ_ID_NAVI_SATELLITE,SatelliteHandler,CNaviSatelliteUIC::factory);
        g_pServer->registerService(REQ_ID_NAVI_SHUTDOWN,ShutDownHandler,CNavShutDownReqC::factory);

        //Gui register
        g_pServer->registerService(REQ_ID_GUI_DAYNIGHTMODE,GuiDayNightModeHandler,CGuiDayNightModeReqC::factory);

        printf("FILE ********************************************%s[%d]:\n",__FILE__,__LINE__);

        if(g_pServer->connect()<0)
        {
            printf("DBusServerC Failed to connect to DBUS server.\r\n");
            //return false;
        }

        IBD_INIT("gui", g_pServer);
    }
    else
    {
        printf("DBusServerC Failed to initialize.\r\n");
    }
    DBusClientProxyC client((char*)"com.innov.jt808", (char*)"/jt808");
    g_pClient = &client;
    if(NULL != g_pClient)
    {
        g_pClient->registerNotify(NOTF_ID_NAVI_ELECTRONIC_FENCE,CNaviElectronicFenceNotifyHandler,CNaviElectronicFenceNotifyC::factory);
        if(g_pClient->connect()<0)
        {
            printf("DBusClientProxyC Failed to connect to DBUS server.\r\n");
        }
    }
    else
    {
        printf("DBusClientProxyC Failed to initialize.\r\n");
    }
    DBusClientProxyC* g_pAudioClient = new DBusClientProxyC(IBAC_AUDIO_SERVICE_NAME, IBAC_AUDIO_SERVER_NAME);

    AudioCtrlClientC *sp_pAudioClientCtl=new AudioCtrlClientC(g_pAudioClient);

    if(g_pAudioClient->connect()<0)
    {
        printf("DBusClientProxyC g_pAudioClient Failed to connect to DBUS server.\r\n");
    }
    int ret = sp_pAudioClientCtl->openAudio(IBAC_DEV_ID_DAI_GPS, 255);

    s_pfencemanager->init();
#endif

    engineCreateContext();
    engineInitSystem();

#if defined(ARM_LINUX)
    g_satelliteModule->openGPS();
#else
    g_satelliteModule->openSimGPS();
#endif

//    GuideThread gd;
//    gd.start();
    MapWindow w;
    w.show();

//    MapView* p_mapview = new MapView(&w);
//    p_mapview->move(0,0);
//    p_mapview->show();

    g_mainwindow = &w;
#if defined(ARM_LINUX)
    g_pWindowManager = new WindowManagerApp(&w,"Navi",g_pServer,g_pClient);
#endif
    return a.exec();
}
