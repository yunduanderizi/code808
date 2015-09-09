#include "menu_search.h"
#include "engineinterface/interface_search.h"
#include "windowinterface.h"
#include "globaldefine.h"
Menu_Search::Menu_Search(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    ui.pic_title->setText(UI_SEARCH_MENU);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_keyword,SIGNAL(clicked()),this,SLOT(keywordSlot()),Qt::QueuedConnection);
    connect(ui.btn_around,SIGNAL(clicked()),this,SLOT(searcharoundSlot()),Qt::QueuedConnection);
    connect(ui.btn_cross,SIGNAL(clicked()),this,SLOT(searchcrossSlot()),Qt::QueuedConnection);
    connect(ui.btn_category,SIGNAL(clicked()),this,SLOT(searchcategorySlot()),Qt::QueuedConnection);

}

Menu_Search::~Menu_Search()
{
}

void Menu_Search::searcharoundSlot()
{
    searchInit();
    this->close();changewindow(AROUNDMENU);
}

void Menu_Search::keywordSlot()
{
    searchInit();
    searchSelectObjHandleInAdmin();
    this->close();changewindow(KEYWORDMENU);
}

void Menu_Search::backSlot()
{
    this->close();gotoMap();
}

void Menu_Search::searchcrossSlot()
{
    searchInit();
    searchSelectObjHandleInAdmin();
    this->close();changewindow(ROADMENU);
}

void Menu_Search::searchcategorySlot()
{
    searchInit();
    searchSelectObjHandleInAdmin();
    this->close();changewindow(CATEGORYMENU);
}
