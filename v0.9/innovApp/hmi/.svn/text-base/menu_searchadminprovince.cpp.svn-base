#include "menu_searchadminprovince.h"
#include "engineinterface/interface_search.h"
#include "windowinterface.h"
#include "globaldefine.h"
#include <QDebug>
Menu_SearchAdminProvice::Menu_SearchAdminProvice(QWidget* parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);
    ui.pic_title->setText(UI_ADMIN_MENU);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_pageup,SIGNAL(clicked()),this,SLOT(upPageSlot()),Qt::QueuedConnection);
    connect(ui.btn_pagedowm,SIGNAL(clicked()),this,SLOT(downPageSlot()),Qt::QueuedConnection);

    initPage();
    initAdminLabel();
}

void Menu_SearchAdminProvice::initPage()
{
    int nCount = searchGetResultCount();
    curPos = 0;
    printOnePage(nCount);
}

void Menu_SearchAdminProvice::printOnePage(int nCurrentPageSum)
{
    ui.lst_resultView->clear();
    structDetailInfo structInfo;

    for(int i = 0;i < nCurrentPageSum; i++)
    {
        //const WCHAR *tmp = getResultList(i, TEXT_TYPE_NAME, 0);
        if(searchGetRetTextName(i,&structInfo))
        {
            QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
            ui.lst_resultView->addItem(str);
        }
    }
}

void Menu_SearchAdminProvice::upPageSlot()
{
    curPos-=PAGE_ITEM_SUM;
    if(curPos < 0)
        curPos = 0;
    ui.lst_resultView->scrollTo(ui.lst_resultView->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchAdminProvice::downPageSlot()
{
    int nCount = searchGetResultCount();
    curPos+=PAGE_ITEM_SUM;
    if(curPos >= nCount)
        curPos = nCount - 1;
    ui.lst_resultView->scrollTo(ui.lst_resultView->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchAdminProvice::initAdminLabel()
{
    const WCHAR *state = searchGetProvinceName();
    QString statestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(state)));
    const WCHAR *place = searchGetCityName();
    QString placestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(place)));
    QStringList listStr = placestr.split(QRegExp(","));
    ui.pic_state->setText(statestr);
    ui.pic_place->setText(listStr[0]);
}

Menu_SearchAdminProviceCross::Menu_SearchAdminProviceCross(QWidget* parent)
    :Menu_SearchAdminProvice(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlotCross()),Qt::QueuedConnection);
    connect(ui.lst_resultView,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlotCross(QListWidgetItem*)), Qt::QueuedConnection);
}

void Menu_SearchAdminProviceCross::backSlotCross()
{
    searchInit();
    searchSelectObjHandleInAdmin();
    this->close();changewindow(ROADMENU);
}

void Menu_SearchAdminProviceCross::itemSelectSlotCross(QListWidgetItem*)
{
    int idx = ui.lst_resultView->currentRow();
    searchSelectObjHandleInSearchResult(idx);
    searchSetTmpProvinceIdx(idx);
    searchSetTmpProvinceName();

    searchInitCity();
    this->close();changewindow(CROSSCITY);

}

Menu_SearchAdminProviceKeyWord::Menu_SearchAdminProviceKeyWord(QWidget* parent)
    :Menu_SearchAdminProvice(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlotKeyWord()),Qt::QueuedConnection);
    connect(ui.lst_resultView,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlotKeyWord(QListWidgetItem*)), Qt::QueuedConnection);
}

void Menu_SearchAdminProviceKeyWord::backSlotKeyWord()
{
    searchInit();
    searchSelectObjHandleInAdmin();
    this->close();changewindow(KEYWORDMENU);
}

void Menu_SearchAdminProviceKeyWord::itemSelectSlotKeyWord(QListWidgetItem*)
{
    int idx = ui.lst_resultView->currentRow();
    searchSelectObjHandleInSearchResult(idx);
    searchSetTmpProvinceIdx(idx);
    searchSetTmpProvinceName();

    searchInitCity();
    this->close();changewindow(KEYWORDADMINCITY);
}

Menu_SearchAdminProviceCategory::Menu_SearchAdminProviceCategory(QWidget* parent)
    :Menu_SearchAdminProvice(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlotCategory()),Qt::QueuedConnection);
    connect(ui.lst_resultView,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlotCategory(QListWidgetItem*)), Qt::QueuedConnection);
}

void Menu_SearchAdminProviceCategory::backSlotCategory()
{
    searchInit();
    searchSelectObjHandleInAdmin();
    this->close();changewindow(CATEGORYRESULTLIST);
}

void Menu_SearchAdminProviceCategory::itemSelectSlotCategory(QListWidgetItem*)
{
    int idx = ui.lst_resultView->currentRow();
    searchSelectObjHandleInSearchResult(idx);
    searchSetTmpProvinceIdx(idx);    //saved selected state temporarily
    searchSetTmpProvinceName();

    searchInitCity();
    this->close();changewindow(CATEGORYCITY);

}

