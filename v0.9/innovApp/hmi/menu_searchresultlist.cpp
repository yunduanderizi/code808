#include "menu_searchresultlist.h"
#include "engineinterface/interface_search.h"
#include "windowinterface.h"
#include "csingalobject.h"
#include "globaldefine.h"
#include "msgbox_onebtn.h"
Menu_SearchResultList::Menu_SearchResultList(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    ui.uIPicture_2->setText(UI_RESULT_LIST_MENU);

}

Menu_SearchResultList::~Menu_SearchResultList()
{

}

Menu_SearchResultListKey::Menu_SearchResultListKey(QWidget *parent)
    :Menu_SearchResultList(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.lst_results,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlot(QListWidgetItem*)), Qt::QueuedConnection);
    connect(ui.btn_pageup,SIGNAL(clicked()),this,SLOT(upPageSlot()),Qt::QueuedConnection);
    connect(ui.btn_pagedown,SIGNAL(clicked()),this,SLOT(downPageSlot()),Qt::QueuedConnection);

    initPage();
}

Menu_SearchResultListKey::~Menu_SearchResultListKey()
{
}

void Menu_SearchResultListKey::initPage()
{
    curPos = 0;
    int nCount = searchGetPoiResultCount();
    printOnePage(nCount);
}

void Menu_SearchResultListKey::printOnePage(int nCurrentPageSum)
{
    ui.lst_results->clear();
    structDetailInfo structInfo;

    for(int i = 0;i < nCurrentPageSum; i++)
    {
        if(searchGetMidPoiRetInfo(i,&structInfo))
        {
            QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
            ui.lst_results->addItem(str + "(" + SINGALMODULE->length2String(structInfo.uDistance) + ")");
        }
    }     
}

void Menu_SearchResultListKey::backSlot()
{
    searchInit();
    searchSelectObjHandleInAdmin();
    this->close();changewindow(KEYWORDMENU);

}

void Menu_SearchResultListKey::upPageSlot()
{
    curPos-=PAGE_ITEM_SUM;
    if(curPos < 0)
        curPos = 0;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchResultListKey::downPageSlot()
{
    int nCount = searchGetPoiResultCount();
    curPos+=PAGE_ITEM_SUM;
    if(curPos >= nCount)
        curPos = nCount - 1;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchResultListKey::itemSelectSlot(QListWidgetItem*)
{
    int idx = getLstSelItemIdx();
    searchSetSelectIndex(idx);
    this->close();changewindow(KEYRESULTINFO);

}

int Menu_SearchResultListKey::getLstSelItemIdx()
{
    return ui.lst_results->currentRow();
}

Menu_SearchResultListAround::Menu_SearchResultListAround(QWidget *parent)
    :Menu_SearchResultList(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.lst_results,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlot(QListWidgetItem*)), Qt::QueuedConnection);
    connect(ui.btn_pageup,SIGNAL(clicked()),this,SLOT(upPageSlot()),Qt::QueuedConnection);
    connect(ui.btn_pagedown,SIGNAL(clicked()),this,SLOT(downPageSlot()),Qt::QueuedConnection);

    initPage();
}

Menu_SearchResultListAround::~Menu_SearchResultListAround()
{

}

void Menu_SearchResultListAround::initPage()
{
    curPos = 0;
    int nCount = searchGetPoiResultCount();
    printOnePage(nCount);
}

void Menu_SearchResultListAround::printOnePage(int nCurrentPageSum)
{
    ui.lst_results->clear();
    structDetailInfo structInfo;

    for(int i = 0;i < nCurrentPageSum; i++)
    {
        if(searchGetMidPoiRetInfo(i,&structInfo))
        {
            QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
            ui.lst_results->addItem(str + "(" + SINGALMODULE->length2String(structInfo.uDistance) + ")");
        }
    }
}

void Menu_SearchResultListAround::upPageSlot()
{
    curPos-=PAGE_ITEM_SUM;
    if(curPos < 0)
        curPos = 0;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchResultListAround::downPageSlot()
{
    int nCount = searchGetPoiResultCount();
    curPos+=PAGE_ITEM_SUM;
    if(curPos >= nCount)
        curPos = nCount - 1;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchResultListAround::backSlot()
{
    this->close();changewindow(AROUNDMENU);
}

void Menu_SearchResultListAround::itemSelectSlot(QListWidgetItem*)
{

    int idx = getLstSelItemIdx();
    searchSetSelectIndex(idx);
    this->close();changewindow(AROUNDRESULTINFO);
}

int Menu_SearchResultListAround::getLstSelItemIdx()
{
    return ui.lst_results->currentRow();
}

Menu_SearchResultListRoad::Menu_SearchResultListRoad(QWidget *parent)
    :Menu_SearchResultList(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.lst_results,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlot(QListWidgetItem*)), Qt::QueuedConnection);
    connect(ui.btn_pageup,SIGNAL(clicked()),this,SLOT(upPageSlot()),Qt::QueuedConnection);
    connect(ui.btn_pagedown,SIGNAL(clicked()),this,SLOT(downPageSlot()),Qt::QueuedConnection);

    initPage();
}

Menu_SearchResultListRoad::~Menu_SearchResultListRoad()
{
}

void Menu_SearchResultListRoad::initPage()
{
    curPos = 0;
    int nCount = searchGetResultCount();
    printOnePage(nCount);
}

void Menu_SearchResultListRoad::printOnePage(int nCurrentPageSum)
{
    ui.lst_results->clear();
    structDetailInfo structInfo;
    for(int i = 0;i < nCurrentPageSum; i++)
    {
        if(searchGetRetTextName(i,&structInfo))
        {
            QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
            ui.lst_results->addItem(str);
        }
    }
}

void Menu_SearchResultListRoad::backSlot()
{
    searchInit();
    searchSelectObjHandleInAdmin();
    this->close();changewindow(ROADMENU);
}

void Menu_SearchResultListRoad::itemSelectSlot(QListWidgetItem*)
{
    searchSelectObjHandleInSearchResult(ui.lst_results->currentRow());
    searchSetRoadName(ui.lst_results->currentRow());
    fromRoad2Cross();

    if(searchGetResultCount() > 0)
    {
        this->close();changewindow(CROSSLIST);
    }
    else
    {
        MsgBoxSearchNoItem* w = new MsgBoxSearchNoItem(this);
        w->show();
    }
}

void Menu_SearchResultListRoad::upPageSlot()
{
    curPos-=PAGE_ITEM_SUM;
    if(curPos < 0)
        curPos = 0;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchResultListRoad::downPageSlot()
{
    int nCount = searchGetResultCount();
    curPos+=PAGE_ITEM_SUM;
    if(curPos >= nCount)
        curPos = nCount - 1;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchResultListRoad::fromRoad2Cross()
{
    searchSetLevel(SCH_CROSS_LOAD_LEVEL);
    searchSetType(SCH_DEFAUT_SEARCH_TYPE);
    searchCreateQuery((WCHAR*)NULL);
    searchExecuteQuery();
}

int Menu_SearchResultListRoad::getLstSelItemIdx()
{
    return ui.lst_results->currentRow();
}


Menu_SearchResultListCross::Menu_SearchResultListCross(QWidget *parent)
    :Menu_SearchResultList(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.lst_results,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlot(QListWidgetItem*)), Qt::QueuedConnection);
    connect(ui.btn_pageup,SIGNAL(clicked()),this,SLOT(upPageSlot()),Qt::QueuedConnection);
    connect(ui.btn_pagedown,SIGNAL(clicked()),this,SLOT(downPageSlot()),Qt::QueuedConnection);

    initPage();
}

Menu_SearchResultListCross::~Menu_SearchResultListCross()
{
}

void Menu_SearchResultListCross::initPage()
{
    curPos = 0;
    int nCount = searchGetResultCount();
    printOnePage(nCount);
}

void Menu_SearchResultListCross::printOnePage(int nCurrentPageSum)
{
    ui.lst_results->clear();
    structDetailInfo structInfo;

    for(int i = 0;i < nCurrentPageSum; i++)
    {
        if(searchGetRetTextName(i,&structInfo))
        {
            QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
            ui.lst_results->addItem(str);
        }
    }
}

void Menu_SearchResultListCross::backSlot()
{
    searchReset();
    searchSelectObjHandleInAdmin();

    searchSetLevel(SCH_ROAD_LEVEL);
    searchSetType(SCH_DEFAUT_SEARCH_TYPE);
    QString key = CSingalObject::instance()->getStrInputBakeup();
    const WCHAR * searchText = reinterpret_cast<const WCHAR *>(key.utf16());
    searchCreateQuery(searchText);
    searchExecuteQuery();

    this->close();changewindow(ROADLIST);

}

void Menu_SearchResultListCross::itemSelectSlot(QListWidgetItem*)
{

    int idx = getLstSelItemIdx();
    searchSetSelectIndex(idx);
    this->close();changewindow(CROSSRESULTINFO);
}

void Menu_SearchResultListCross::upPageSlot()
{
    curPos-=PAGE_ITEM_SUM;
    if(curPos < 0)
        curPos = 0;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);

}

void Menu_SearchResultListCross::downPageSlot()
{
    int nCount = searchGetResultCount();
    curPos+=PAGE_ITEM_SUM;
    if(curPos >= nCount)
        curPos = nCount - 1;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

int Menu_SearchResultListCross::getLstSelItemIdx()
{
    return ui.lst_results->currentRow();
}








