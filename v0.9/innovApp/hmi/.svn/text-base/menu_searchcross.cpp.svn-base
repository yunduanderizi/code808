#include "menu_searchcross.h"
#include "engineinterface/interface_search.h"
#include "msgbox_onebtn.h"
#include "windowinterface.h"
#include "csingalobject.h"
#include "globaldefine.h"

Menu_SearchCross::Menu_SearchCross(QWidget *parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);
    connect(ui.btn_ok,SIGNAL(clicked()),this,SLOT(searchSlot()),Qt::QueuedConnection);
    connect(ui.btn_admin,SIGNAL(clicked()),this,SLOT(adminSlot()),Qt::QueuedConnection);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    ui.pic_title->setText(UI_CROSS_MENU);

    initAdminLabel();
}

Menu_SearchCross::~Menu_SearchCross()
{
}

void Menu_SearchCross::backSlot()
{
    this->close();changewindow(SEARCHMENU);

}

void Menu_SearchCross::adminSlot()
{
    searchInitProvince();
    this->close();changewindow(CROSSPROVICE);

}

void Menu_SearchCross::search()
{
    searchExecuteQuery();
}

void Menu_SearchCross::startsearch()
{
 //    ui.item_resultList->clear();
    QString key = ui.edit_name->text();

    const WCHAR * searchText = reinterpret_cast<const WCHAR *>(key.utf16());
    searchSetType(SCH_DEFAUT_SEARCH_TYPE);
    searchCreateQuery(searchText);
    CSingalObject::instance()->setStrInputBakeup(key);
    searchExecuteQuery();
}

void Menu_SearchCross::searchSlot()
{
    searchSetLevel(SCH_ROAD_LEVEL);
    startsearch();

    if(searchGetResultCount() > 0)
    {
//        QString key = ui.edit_name->text();

        this->close();changewindow(ROADLIST);  //todo
    }
    else
    {
        MsgBoxSearchNoItem* w = new MsgBoxSearchNoItem(this);
        w->show();
    }
}

void Menu_SearchCross::initAdminLabel()
{
    const WCHAR *state = searchGetProvinceName();
    QString statestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(state)));
    const WCHAR *place = searchGetCityName();
    QString placestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(place)));
    QStringList listStr = placestr.split(QRegExp(","));
    ui.btn_admin->setText(statestr + " : " + listStr[0]);
}
