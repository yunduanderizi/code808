#include "menu_searchadmincity.h"
#include "engineinterface/interface_search.h"
#include "windowinterface.h"
#include "globaldefine.h"

Menu_SearchAdminCity::Menu_SearchAdminCity(QWidget* parent)
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

void Menu_SearchAdminCity::initPage()
{
    curPos = 0;
    int nCount = searchGetResultCount();
        printOnePage(nCount);
}

void Menu_SearchAdminCity::printOnePage(int nCurrentPageSum)
{
    ui.lst_resultView->clear();
    structDetailInfo structInfo;

    for(int i = 0;i < nCurrentPageSum; i++)
    {
        //const WCHAR *tmp = getResultList(i, TEXT_TYPE_NAME, 0);
        if(searchGetRetTextName(i,&structInfo))
        {
            QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(structInfo.name)));
            QStringList listStr = str.split(QRegExp(","));
            ui.lst_resultView->addItem(listStr[0]);
        }
    }
}

void Menu_SearchAdminCity::upPageSlot()
{
    curPos-=PAGE_ITEM_SUM;
    if(curPos < 0)
        curPos = 0;
    ui.lst_resultView->scrollTo(ui.lst_resultView->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchAdminCity::downPageSlot()
{
    int nCount = searchGetResultCount();
    curPos+=PAGE_ITEM_SUM;
    if(curPos >= nCount)
        curPos = nCount - 1;
    ui.lst_resultView->scrollTo(ui.lst_resultView->model()->index(curPos,0),QAbstractItemView::PositionAtTop);
}

void Menu_SearchAdminCity::initAdminLabel()
{
    const WCHAR *state = searchGetTmpProvinceName();
    QString statestr(QString::fromUtf16(reinterpret_cast<const unsigned short *>(state)));
    ui.pic_state->setText(statestr);
    ui.pic_place->setText("");
}

Menu_SearchAdminCityCross::Menu_SearchAdminCityCross(QWidget* parent)
    :Menu_SearchAdminCity(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlotCross()),Qt::QueuedConnection);
    connect(ui.lst_resultView,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlotCross(QListWidgetItem*)), Qt::QueuedConnection);
}

void Menu_SearchAdminCityCross::backSlotCross()
{
    searchInitProvince();
    this->close();changewindow(CROSSPROVICE);
}

void Menu_SearchAdminCityCross::itemSelectSlotCross(QListWidgetItem*)
{
    int idx = ui.lst_resultView->currentRow();
    searchSelectObjHandleInSearchResult(idx);
    searchsetCityIdx(idx);
    searchSetCityName();

    this->close();changewindow(ROADMENU);

}

Menu_SerachAdminCityKeyWord::Menu_SerachAdminCityKeyWord(QWidget* parent)
    :Menu_SearchAdminCity(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlotKeyWord()),Qt::QueuedConnection);
    connect(ui.lst_resultView,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlotKeyWord(QListWidgetItem*)), Qt::QueuedConnection);
}

void Menu_SerachAdminCityKeyWord::backSlotKeyWord()
{
    searchInitProvince();
    this->close();changewindow(KEYWORDADMINPROVICE);

}

void Menu_SerachAdminCityKeyWord::itemSelectSlotKeyWord(QListWidgetItem*)
{
    int idx = ui.lst_resultView->currentRow();
    searchSelectObjHandleInSearchResult(idx);
    searchsetCityIdx(idx);
//    searchInit();
//    searchSelectObjHandleInAdmin();
    searchSetCityName();                    //saved selected state temporarily

    this->close();changewindow(KEYWORDMENU);
}

Menu_SearchAdminCityCategory::Menu_SearchAdminCityCategory(QWidget* parent)
    :Menu_SearchAdminCity(parent)
{
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlotCategory()),Qt::QueuedConnection);
    connect(ui.lst_resultView,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlotCategory(QListWidgetItem*)), Qt::QueuedConnection);
}

void Menu_SearchAdminCityCategory::backSlotCategory()
{
    searchInitProvince();
    this->close();changewindow(CATEGORYPROVICE);

}

void Menu_SearchAdminCityCategory::itemSelectSlotCategory(QListWidgetItem*)
{
    int idx = ui.lst_resultView->currentRow();
    searchSelectObjHandleInSearchResult(idx);
    searchsetCityIdx(idx);
    searchSetCityName();

    searchResetPoiResultCount();
    searchSetLevel(SCH_POI_LEVEL);
    searchSetType(SCH_POI_CATEGORY_SEARCH_TYPE);
    searchCreateQuery((WCHAR *)NULL);
    searchExecuteQuery();

    this->close();changewindow(CATEGORYRESULTLIST);

}
