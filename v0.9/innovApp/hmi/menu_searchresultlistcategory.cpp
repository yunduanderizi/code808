#include "menu_searchresultlistcategory.h"
#include "engineinterface/interface_search.h"
#include "windowinterface.h"
#include "csingalobject.h"
#include "globaldefine.h"

Menu_SearchResultListCategory::Menu_SearchResultListCategory(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    ui.pic_title->setText(UI_RESULT_LIST_MENU);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_admin,SIGNAL(clicked()),this,SLOT(adminSlot()),Qt::QueuedConnection);
    connect(ui.lst_results,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlot(QListWidgetItem*)), Qt::QueuedConnection);
    connect(ui.btn_pageup,SIGNAL(clicked()),this,SLOT(upPageSlot()),Qt::QueuedConnection);
    connect(ui.btn_pagedown,SIGNAL(clicked()),this,SLOT(downPageSlot()),Qt::QueuedConnection);

    initAdminLabel();
    initPage();
}

Menu_SearchResultListCategory::~Menu_SearchResultListCategory()
{
}

void Menu_SearchResultListCategory::initPage()
{
    curPos = 0;
    int nCount = searchGetPoiResultCount();
    printOnePage(nCount);
}

void Menu_SearchResultListCategory::printOnePage(int nCurrentPageSum)
{
    ui.lst_results->clear();
    structDetailInfo structInfo;

    for(int i = 0;i < nCurrentPageSum; i++)
    {
        if(searchGetMidPoiRetInfo(i,&structInfo))
        {
            QString str(QString::fromUtf16(structInfo.name));
            ui.lst_results->addItem(str + "(" + SINGALMODULE->length2String(structInfo.uDistance) + ")");
        }
    }
}

void Menu_SearchResultListCategory::upPageSlot()
{
    curPos-=PAGE_ITEM_SUM;
    if(curPos < 0)
        curPos = 0;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchResultListCategory::downPageSlot()
{
    int nCount = searchGetPoiResultCount();
    curPos+=PAGE_ITEM_SUM;
    if(curPos >= nCount)
        curPos = nCount - 1;
    ui.lst_results->scrollTo(ui.lst_results->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchResultListCategory::initAdminLabel()
{
    const WCHAR *state = searchGetProvinceName();
    QString statestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(state)));
    const WCHAR *place = searchGetCityName();
    QString placestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(place)));
    QStringList listStr = placestr.split(QRegExp(","));
    ui.btn_admin->setText(statestr + " : " + listStr[0]);
}

void Menu_SearchResultListCategory::adminSlot()
{
    searchInitProvince();
    this->close();changewindow(CATEGORYPROVICE);
}

void Menu_SearchResultListCategory::backSlot()
{
    this->close();changewindow(CATEGORYMENU);
}

void Menu_SearchResultListCategory::itemSelectSlot(QListWidgetItem*)
{

    int idx = getLstSelItemIdx();
    searchSetSelectIndex(idx);
    this->close();changewindow(CATEGORYRESULTINFO);
}

int Menu_SearchResultListCategory::getLstSelItemIdx()
{
    return ui.lst_results->currentRow();
}

