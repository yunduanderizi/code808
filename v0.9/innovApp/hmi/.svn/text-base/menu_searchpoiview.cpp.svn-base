#include "menu_searchpoiview.h"
#include "windowinterface.h"
#include "engineinterface/interface_search.h"
#include "engineinterface/interface_map.h"
#include <QDebug>

Menu_SearchPoiView::Menu_SearchPoiView(QWidget *parent) :
    NaviBaseWnd(parent)
{
    if(mapGetMapMode() != MAPMODE_2D_NORTH)
    {
        mapSetMapMode(MAPMODE_2D_NORTH);
    }
    ui.setupUi(this);
    connect(ui.btn_return,SIGNAL(clicked()),this,SLOT(returnSlot()),Qt::QueuedConnection);
}

Menu_SearchPoiView::~Menu_SearchPoiView()
{

}

Menu_SearchPoiViewKey::Menu_SearchPoiViewKey(QWidget* parent)
    :Menu_SearchPoiView(parent)
{
}

Menu_SearchPoiViewKey::~Menu_SearchPoiViewKey()
{
}

void Menu_SearchPoiViewKey::returnSlot()
{
    searchClearFavourites();
    this->close();changewindow(KEYRESULTINFO);

}

Menu_SearchPoiViewAround::Menu_SearchPoiViewAround(QWidget* parent)
    :Menu_SearchPoiView(parent)
{
}

Menu_SearchPoiViewAround::~Menu_SearchPoiViewAround()
{
}

void Menu_SearchPoiViewAround::returnSlot()
{
    searchClearFavourites();
    this->close();changewindow(AROUNDRESULTINFO);

}

Menu_SearchPoiViewCategory::Menu_SearchPoiViewCategory(QWidget* parent)
    :Menu_SearchPoiView(parent)
{
}

Menu_SearchPoiViewCategory::~Menu_SearchPoiViewCategory()
{
}

void Menu_SearchPoiViewCategory::returnSlot()
{
    searchClearFavourites();
    this->close();changewindow(CATEGORYRESULTINFO);

}

Menu_SearchPoiViewCross::Menu_SearchPoiViewCross(QWidget* parent)
    :Menu_SearchPoiView(parent)
{
}

Menu_SearchPoiViewCross::~Menu_SearchPoiViewCross()
{
}

void Menu_SearchPoiViewCross::returnSlot()
{
    searchClearFavourites();
    this->close();changewindow(CROSSRESULTINFO);

}
