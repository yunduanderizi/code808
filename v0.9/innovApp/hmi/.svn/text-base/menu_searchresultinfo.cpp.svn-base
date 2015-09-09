#include "menu_searchresultinfo.h"
#include "engineinterface/interface_search.h"
#include "engineinterface/engineinterface.h"
#include "msgbox_threebtn.h"
#include "menu_routeconfirm.h"
#include "windowinterface.h"
#include "globaldefine.h"
#include "msgbox_onebtn.h"
Menu_SearchResultInfo::Menu_SearchResultInfo(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    ui.pic_title->setText(UI_RESULT_INFO_MENU);
    ui.btn_mark->enabled(false);
}

Menu_SearchResultInfo::~Menu_SearchResultInfo()
{

}

Menu_SearchResultInfoKey::Menu_SearchResultInfoKey(QWidget* parent)
    :Menu_SearchResultInfo(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_view,SIGNAL(clicked()),this,SLOT(viewSlot()),Qt::QueuedConnection);
    connect(ui.btn_guide,SIGNAL(clicked()),this,SLOT(guideSlot()),Qt::QueuedConnection);
    initResultInfo();
}

Menu_SearchResultInfoKey::~Menu_SearchResultInfoKey()
{
}

void Menu_SearchResultInfoKey::initResultInfo()
{
    int i = searchGetSelectIndex();
    structDetailInfo structInfo;
    QString strDist = "";
    QString strTextAdress = RESULT_ADDRESS;
    QString strTextTel = RESULT_PHONE;
    QString strTextDistrict= RESULT_SECTION;

    if(searchGetMidPoiRetInfo(i,&structInfo))
    {
        QString strName(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
        QString strAdress(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.address)));
        QString strTel(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.tel)));

        ui.pic_info->setText( strName );
        QStringList listStr = strAdress.split(QRegExp("\\n+"));
        if(listStr.size() >= 2)
        {
            strDist = listStr[1];
        }
        strAdress.remove(QChar('\n'));
        ui.pic_name->setText(strTextAdress+strAdress);
        ui.pic_tel->setText(strTextTel+strTel);
        ui.pic_state->setText(strTextDistrict + strDist);
//        ui.uipicture->setText(strTextAdress+ strAdress + '\n' +
//                              strTextTel + strTel + '\n' +
//                              strTextDistrict + strDist);
    }
}

void Menu_SearchResultInfoKey::backSlot()
{
    this->close();changewindow(KEYWORDLIST);

}

void Menu_SearchResultInfoKey::viewSlot()
{
   // showmap();
    searchShowMapPoi();
    this->close();changewindow(KEYWORDPOIVIEW);
}

void Menu_SearchResultInfoKey::guideSlot()
{
    if(searchResultGuidePOI())
    {
        MsgBox_RoutePOI *w = new MsgBox_RoutePOI(this);
        connect(w, SIGNAL(closefatherWnd()), this, SLOT(gotonextwindow()));
        w->show();

    }
    else
    {
        MsgBoxCalculatFailPath* w = new MsgBoxCalculatFailPath(this);
        w->show();
    }
}

void Menu_SearchResultInfoKey::gotonextwindow()
{
    this->close();gotoMap();
}

Menu_SearchResultInfoAround::Menu_SearchResultInfoAround(QWidget* parent)
    :Menu_SearchResultInfo(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_view,SIGNAL(clicked()),this,SLOT(viewSlot()),Qt::QueuedConnection);
    connect(ui.btn_guide,SIGNAL(clicked()),this,SLOT(guideSlot()),Qt::QueuedConnection);

    initResultInfo();
}

Menu_SearchResultInfoAround::~Menu_SearchResultInfoAround()
{
}

void Menu_SearchResultInfoAround::initResultInfo()
{
    int i = searchGetSelectIndex();
    structDetailInfo structInfo;
    QString strDist = "";
    QString strTextAdress = RESULT_ADDRESS;
    QString strTextTel = RESULT_PHONE;
    QString strTextDistrict= RESULT_SECTION;

    if(searchGetMidPoiRetInfo(i,&structInfo))
    {
        QString strName(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
        QString strAdress(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.address)));
        QString strTel(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.tel)));

        ui.pic_info->setText( strName );
        QStringList listStr = strAdress.split(QRegExp("\\n+"));
        if(listStr.size() >= 2)
        {
            strDist = listStr[1];
        }
        strAdress.remove(QChar('\n'));
        ui.pic_name->setText(strTextAdress+strAdress);
        ui.pic_tel->setText(strTextTel+strTel);
        ui.pic_state->setText(strTextDistrict + strDist);
//        ui.uipicture->setText(strTextAdress+ strAdress + '\n' +
//                              strTextTel + strTel + '\n' +
//                              strTextDistrict + strDist);
    }
}

void Menu_SearchResultInfoAround::backSlot()
{
    this->close();changewindow(AROUNDLIST);

}

void Menu_SearchResultInfoAround::viewSlot()
{
    searchShowMapPoi();
    this->close();changewindow(AROUNDPOIVIEW);
}

void Menu_SearchResultInfoAround::guideSlot()
{
    if(searchResultGuidePOI())
    {
        MsgBox_RoutePOI *w = new MsgBox_RoutePOI(this);
        connect(w, SIGNAL(closefatherWnd()), this, SLOT(gotonextwindow()));
        w->show();

    }
    else
    {
        MsgBoxCalculatFailPath* w = new MsgBoxCalculatFailPath(this);
        w->show();
    }
}

void Menu_SearchResultInfoAround::gotonextwindow()
{
    this->close();gotoMap();
}


Menu_SearchResultInfoCross::Menu_SearchResultInfoCross(QWidget* parent)
    :Menu_SearchResultInfo(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_view,SIGNAL(clicked()),this,SLOT(viewSlot()),Qt::QueuedConnection);
    connect(ui.btn_guide,SIGNAL(clicked()),this,SLOT(guideSlot()),Qt::QueuedConnection);
    initResultInfo();
}

void Menu_SearchResultInfoCross::initResultInfo()
{
    structDetailInfo structInfo;
    if(searchGetRetTextName(searchGetSelectIndex(),&structInfo))
    {
        QString strDist = "";
        QString strTextDistrict= RESULT_SECTION;
        QString strRoad(QString::fromUtf16(reinterpret_cast<const unsigned short *>(searchGetRoadName())));
        QString strTemp(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));

        QStringList listStrRoad = strRoad.split(QRegExp("\\W+"), QString::SkipEmptyParts);
        if(listStrRoad.size() == 4)
        {
            strDist = listStrRoad[listStrRoad.size()- 2];
        }
        else if(listStrRoad.size() == 3)
        {
            strDist = listStrRoad[listStrRoad.size()- 1];
        }
        QStringList listStrTemp = strTemp.split(QRegExp("\\W+"), QString::SkipEmptyParts);
        ui.pic_info->setText(listStrRoad[0] + "," + listStrTemp[0]);
        ui.pic_name->setText(strTextDistrict + strDist);
//        ui.pic_tel->setText(strTextTel+strTel);
//        ui.pic_state->setText(strTextDistrict + strDist);
//        ui.uipicture->setText(strTextDistrict + strDist);
    }
}

void Menu_SearchResultInfoCross::backSlot()
{
    this->close();changewindow(CROSSLIST);

}

void Menu_SearchResultInfoCross::viewSlot()
{
    searchShowMapGeo();
    this->close();changewindow(CROSSPOIVIEW);
}

void Menu_SearchResultInfoCross::guideSlot()
{
    if(searchResultGuideGeo())
    {
        MsgBox_RoutePOI *w = new MsgBox_RoutePOI(this);
         connect(w, SIGNAL(closefatherWnd()), this, SLOT(gotonextwindow()));
         w->show();

    }
    else
    {
        MsgBoxCalculatFailPath* w = new MsgBoxCalculatFailPath(this);
        w->show();
    }
}

void Menu_SearchResultInfoCross::gotonextwindow()
{
    this->close();gotoMap();
}

Menu_SearchResultInfoCategory::Menu_SearchResultInfoCategory(QWidget* parent)
    :Menu_SearchResultInfo(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_view,SIGNAL(clicked()),this,SLOT(viewSlot()),Qt::QueuedConnection);
    connect(ui.btn_guide,SIGNAL(clicked()),this,SLOT(guideSlot()),Qt::QueuedConnection);

    initResultInfo();
}

Menu_SearchResultInfoCategory::~Menu_SearchResultInfoCategory()
{
}

void Menu_SearchResultInfoCategory::initResultInfo()
{
    int i = searchGetSelectIndex();
    structDetailInfo structInfo;
    QString strDist = "";
    QString strTextAdress = RESULT_ADDRESS;
    QString strTextTel = RESULT_PHONE;
    QString strTextDistrict= RESULT_SECTION;

    if(searchGetMidPoiRetInfo(i,&structInfo))
    {
        QString strName(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
        QString strAdress(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.address)));
        QString strTel(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.tel)));

        ui.pic_info->setText( strName );
        QStringList listStr = strAdress.split(QRegExp("\\n+"));
        if(listStr.size() >= 2)
        {
            strDist = listStr[1];
        }
        strAdress.remove(QChar('\n'));
        ui.pic_name->setText(strTextAdress+strAdress);
        ui.pic_tel->setText(strTextTel+strTel);
        ui.pic_state->setText(strTextDistrict + strDist);
//        ui.uipicture->setText(strTextAdress+ strAdress + '\n' +
//                              strTextTel + strTel + '\n' +
//                              strTextDistrict + strDist);
    }
}

void Menu_SearchResultInfoCategory::backSlot()
{
    this->close();changewindow(CATEGORYRESULTLIST);

}

void Menu_SearchResultInfoCategory::viewSlot()
{
    searchShowMapPoi();
    this->close();changewindow(CATEGORYPOIVIEW);
}

void Menu_SearchResultInfoCategory::guideSlot()
{
    if(searchResultGuidePOI())
    {
        MsgBox_RoutePOI *w = new MsgBox_RoutePOI(this);
         connect(w, SIGNAL(closefatherWnd()), this, SLOT(gotonextwindow()));
         w->show();

    }
    else
    {
        MsgBoxCalculatFailPath* w = new MsgBoxCalculatFailPath(this);
        w->show();
    }
}

void Menu_SearchResultInfoCategory::gotonextwindow()
{
    this->close();gotoMap();
}
