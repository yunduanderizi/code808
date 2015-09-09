#include "menu_searchkeyword.h"
#include "engineinterface/interface_search.h"
#include "msgbox_onebtn.h"
#include "windowinterface.h"
#include "globaldefine.h"
Menu_SearchKeyWord::Menu_SearchKeyWord(QWidget *parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);
    connect(ui.btn_ok,SIGNAL(clicked()),this,SLOT(searchSlot()),Qt::QueuedConnection);
    connect(ui.btn_admin,SIGNAL(clicked()),this,SLOT(adminSlot()),Qt::QueuedConnection);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);

    initAdminLabel();
}

Menu_SearchKeyWord::~Menu_SearchKeyWord()
{
}

void Menu_SearchKeyWord::initAdminLabel()
{
    const WCHAR *state = searchGetProvinceName();
    QString statestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(state)));
    const WCHAR *place = searchGetCityName();
    QString placestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(place)));
    QStringList listStr = placestr.split(QRegExp(","));
    ui.btn_admin->setText(statestr + " : " + listStr[0]);
}

void Menu_SearchKeyWord::adminSlot()
{
    searchInitProvince();
    this->close();changewindow(KEYWORDADMINPROVICE);
}

void Menu_SearchKeyWord::backSlot()
{
    this->close();changewindow(SEARCHMENU);
}

void Menu_SearchKeyWord::startsearch()
{
    QString key = ui.edit_name->text();
    const WCHAR * searchText = reinterpret_cast<const WCHAR *>(key.utf16());
    searchSetType(SCH_DEFAUT_SEARCH_TYPE);
    searchCreateQuery(searchText);
    searchExecuteQuery();
}

void Menu_SearchKeyWord::searchSlot()
{
    searchSetLevel(SCH_POI_LEVEL);
    startsearch();

    if(searchGetPoiResultCount() > 0)
    {
        this->close();changewindow(KEYWORDLIST);
    }
    else
    {
        MsgBoxSearchNoItem* w = new MsgBoxSearchNoItem(this);
        w->show();
    }
}
