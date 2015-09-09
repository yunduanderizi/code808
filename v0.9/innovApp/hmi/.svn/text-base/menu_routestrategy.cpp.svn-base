#include "menu_routestrategy.h"
#include "windowinterface.h"
#include  "globaldefine.h"
#include "engineinterface/interface_route.h"


static BOOL8  avoidTollsFlag = FALSE ;
static BOOL8  avoidUnsealedFlag = TRUE ;
static BOOL8  avoidFerriesFlag = TRUE ;
static BOOL8  avoidUturnsFlag = FALSE ;
static BOOL8  avoidFreewayFlag = FALSE ;

static enum RouteStrategy CurrentStrategyType=STRATEGY_FASTEST;
static enum RouteStrategy preservestrategytype = STRATEGY_FASTEST;

Menu_RouteStrategy::Menu_RouteStrategy(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    initOldCheckFlag();
//    ui.cb_avoidtolls->setChecked(avoidTollsFlag);
//    ui.cb_avoidferries->setChecked(avoidFerriesFlag);
//    ui.cb_avoidunsealed->setChecked(avoidUnsealedFlag);
//    ui.cb_avoiduturns->setChecked(avoidUturnsFlag);
//    ui.cb_avoidfreeway->setChecked(avoidFreewayFlag);

    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_strategyleft,SIGNAL(clicked()),this,SLOT(strategyLeftChange()),Qt::QueuedConnection);
    connect(ui.btn_strategyright,SIGNAL(clicked()),this,SLOT(strategyRightChange()),Qt::QueuedConnection);
    //connect(ui.btn_strategy,SIGNAL(clicked()),this,SLOT(setRecommendSlot()),Qt::QueuedConnection);
//    connect(ui.btn_shortest,SIGNAL(clicked()),this,SLOT(setShortestSlot()),Qt::QueuedConnection);
//    connect(ui.btn_fastest,SIGNAL(clicked()),this,SLOT(setFastestSlot()),Qt::QueuedConnection);
//    connect(ui.btn_easiest,SIGNAL(clicked()),this,SLOT(setEasiestSlot()),Qt::QueuedConnection);
//    connect(ui.btn_enconomical,SIGNAL(clicked()),this,SLOT(setEconomicalSlot()),Qt::QueuedConnection);
//    //connect(ui.btn_expressway,SIGNAL(clicked()),this,SLOT(setExpresswaySlot()),Qt::QueuedConnection);
//    connect(ui.btn_saveChange,SIGNAL(clicked()),this,SLOT(saveRoutingOptionsSlot()),Qt::QueuedConnection);
    connect(ui.btn_toll,SIGNAL(clicked()),this,SLOT(updateAvoidTollsSlot()),Qt::QueuedConnection);
//    connect(ui.cb_avoidunsealed,SIGNAL(stateChanged(int)),this,SLOT(updateAvoidUnsealedSlot()),Qt::QueuedConnection);
    connect(ui.btn_ferry,SIGNAL(clicked()),this,SLOT(updateAvoidFerriesSlot()),Qt::QueuedConnection);
//    connect(ui.cb_avoiduturns,SIGNAL(stateChanged(int)),this,SLOT(updateAvoidUturnsSlot()),Qt::QueuedConnection);
    connect(ui.btn_motorway,SIGNAL(clicked()),this,SLOT(updateAvoidFreewaySlot()),Qt::QueuedConnection);
    setStrategyText(CurrentStrategyType);
}

void Menu_RouteStrategy::backSlot()
{
//    ui.cb_avoidtolls->setChecked(oldavoidTollsFlag);
//    ui.cb_avoidferries->setChecked(oldavoidFerriesFlag);
//    ui.cb_avoidunsealed->setChecked(oldavoidUnsealedFlag);
//    ui.cb_avoiduturns->setChecked(oldavoidUturnsFlag);
//    ui.cb_avoidfreeway->setChecked(oldavoidFreewayFlag);
    this->close();changewindow(ROUTEMAINMENU);
}

void Menu_RouteStrategy :: initOldCheckFlag()
{
    oldavoidTollsFlag = avoidTollsFlag;
    oldavoidUnsealedFlag = avoidUnsealedFlag ;
    oldavoidFerriesFlag = avoidFerriesFlag;
    oldavoidUturnsFlag = avoidUturnsFlag;
    oldavoidFreewayFlag = avoidFreewayFlag;
    strategyChanged = FALSE ;
}

void Menu_RouteStrategy :: updateAvoidTollsSlot()
{
    if(!avoidTollsFlag)
    {
        avoidTollsFlag = TRUE;
    }
    else
    {
        avoidTollsFlag  = FALSE;
    }
    routeSetAvoidTolls(avoidTollsFlag,m_options);
}

void Menu_RouteStrategy :: updateAvoidUnsealedSlot()
{
    if(!avoidUnsealedFlag)
    {
        avoidUnsealedFlag = TRUE;
    }
    else
    {
        avoidUnsealedFlag  = FALSE;
    }
}

void Menu_RouteStrategy :: updateAvoidFerriesSlot()
{
    if(!avoidFerriesFlag)
    {
        avoidFerriesFlag = TRUE;
    }
    else
    {
        avoidFerriesFlag  = FALSE;
    }
    routeSetAvoidFerries(avoidFerriesFlag,m_options);
}

void Menu_RouteStrategy :: updateAvoidUturnsSlot()
{
    if(!avoidUturnsFlag)
    {
        avoidUturnsFlag = TRUE;
    }
    else
    {
        avoidUturnsFlag  = FALSE;
    }
  //  routeSetAvoidTolls(avoidUturnsFlag,m_options);
}

void Menu_RouteStrategy :: updateAvoidFreewaySlot()
{
    if(!avoidFreewayFlag)
    {
        avoidFreewayFlag = TRUE;
    }
    else
    {
        avoidFreewayFlag  = FALSE;
    }
    routeSetAvoidFreeway(avoidFreewayFlag,m_options);
}

void Menu_RouteStrategy ::setRecommendSlot()
{
    strategyChanged = TRUE ;
    setRecommend(m_options);
}
void Menu_RouteStrategy ::setRecommend(TRoutingOptions &m_options)
{
    routeSetRecommendRouting(m_options);
}

void Menu_RouteStrategy::setShortestSlot()
{
    strategyChanged = TRUE ;
    setShortest(m_options);

}

void Menu_RouteStrategy::setShortest(TRoutingOptions &m_options)
{
    routeSetShortest(m_options);

}

void Menu_RouteStrategy::setFastestSlot()
{
    strategyChanged = TRUE ;
    setFastest(m_options);
}

void Menu_RouteStrategy::setFastest(TRoutingOptions &m_options)
{
    routeSetFast(m_options);
}

void Menu_RouteStrategy::setEasiestSlot()
{
    strategyChanged = TRUE ;
    setEasiest(m_options);
}

void Menu_RouteStrategy::setEasiest(TRoutingOptions &m_options)
{
    routeSetEasiest(m_options);
}

void Menu_RouteStrategy::setEconomicalSlot()
{
    strategyChanged = TRUE ;
    setEconomical(m_options);
}

void Menu_RouteStrategy::setEconomical(TRoutingOptions &m_options)
{
    routeSetEconomiest(m_options);
}

void Menu_RouteStrategy::setExpresswaySlot()
{
    strategyChanged = TRUE ;
    setExpressway(m_options);
}

void Menu_RouteStrategy::setExpressway(TRoutingOptions &m_options)
{
    routeSetExpressway(m_options);
}

void Menu_RouteStrategy ::setOptions(TRoutingOptions &m_options)
{
//    routeSetAvoidTolls(ui.cb_avoidtolls->isChecked(),m_options);
//    routeSetAvoidFerries(ui.cb_avoidferries->isChecked(),m_options);
//    routeSetAvoidUnsealed(ui.cb_avoidunsealed->isChecked(),m_options);
//    routeSetAvoidUturns(ui.cb_avoiduturns->isChecked(),m_options);
//    routeSetAvoidFreeway(ui.cb_avoidfreeway->isChecked(),m_options);
}

void Menu_RouteStrategy ::calculateRoute()
{
    routeCalculateWhetherOrNot();
}

void Menu_RouteStrategy::strategyLeftChange()
{
//    if(StrategyType!=STRATEGY_SHORTEST)
//    {
//        StrategyType--;
//    }
    if(CurrentStrategyType==STRATEGY_ENCONOMICAL)
    {
        ui.btn_strategyright->enabled(true);
        CurrentStrategyType = STRATEGY_EASIEST;
        setEasiestAvoid();
    }
    else if(CurrentStrategyType ==STRATEGY_EASIEST)
    {
        CurrentStrategyType =STRATEGY_FASTEST;
        setFastestAvoid();
    }
    else if(CurrentStrategyType ==STRATEGY_FASTEST)
    {
        CurrentStrategyType= STRATEGY_SHORTEST;
        ui.btn_strategyleft->enabled(false);
        setShortestAvoid();
    }
    setStrategyText(CurrentStrategyType);
}

void Menu_RouteStrategy::strategyRightChange()
{
//    if(StrategyType!=STRATEGY_ENCONOMICAL)
//    {
//        StrategyType++;
//    }
    if(CurrentStrategyType==STRATEGY_SHORTEST)
    {
        CurrentStrategyType = STRATEGY_FASTEST;
        ui.btn_strategyleft->enabled(true);
        setFastestAvoid();
    }
    else if(CurrentStrategyType ==STRATEGY_FASTEST)
    {
        CurrentStrategyType =STRATEGY_EASIEST;
        setEasiestAvoid();
    }
    else if(CurrentStrategyType ==STRATEGY_EASIEST)
    {
        CurrentStrategyType= STRATEGY_ENCONOMICAL;
        ui.btn_strategyright->enabled(false);
        setEconomicalAvoid();
    }
    setStrategyText(CurrentStrategyType);
}

void Menu_RouteStrategy::setFastestAvoid()
{
    avoidTollsFlag = false;
    avoidUnsealedFlag = true;
    avoidFerriesFlag = true;
    avoidUturnsFlag = false;
}

void Menu_RouteStrategy::setShortestAvoid()
{
    avoidTollsFlag = false;
    avoidUnsealedFlag = true;
    avoidFerriesFlag = true;
    avoidUturnsFlag = false;
}

void Menu_RouteStrategy::setEconomicalAvoid()
{
    avoidTollsFlag = false;
    avoidUnsealedFlag = true;
    avoidFerriesFlag = false;
    avoidUturnsFlag = false;
}

void Menu_RouteStrategy::setEasiestAvoid()
{
    avoidTollsFlag = false;
    avoidUnsealedFlag = true;
    avoidFerriesFlag = true;
    avoidUturnsFlag = true;
}

void Menu_RouteStrategy::setStrategyText(enum RouteStrategy type)
{
    switch (type)
    {
    case STRATEGY_SHORTEST:
        ui.pic_routestrategy->setText(UI_STRATEGY_SHORTEST);
      //  setShortest(m_options);
        break;
    case STRATEGY_FASTEST:
        ui.pic_routestrategy->setText(UI_STRATEGY_FASTEST);
     //   setFastest(m_options);
        break;
    case STRATEGY_ENCONOMICAL:
        ui.pic_routestrategy->setText(UI_STRATEGY_ECONOMICAL);
      //  setEconomical(m_options);
        break;
    case STRATEGY_EASIEST:
        ui.pic_routestrategy->setText(UI_STRATEGY_EASIEST);
      //  setEasiest(m_options);
        break;
    default:
        break;
    }
}

void Menu_RouteStrategy::setStrategy(enum RouteStrategy strategytype)
{
    switch (strategytype)
    {
    case STRATEGY_SHORTEST:
        setShortest(m_options);
        break;
    case STRATEGY_FASTEST:
        setFastest(m_options);
        break;
    case STRATEGY_ENCONOMICAL:
        setEconomical(m_options);
        break;
    case STRATEGY_EASIEST:
        setEasiest(m_options);
        break;
    default:
        break;
    }
}

void Menu_RouteStrategy :: saveRoutingOptions(TRoutingOptions &m_options)
{
    setOptions(m_options);
    if(CurrentStrategyType!=preservestrategytype)
    {
       setStrategy(CurrentStrategyType);
       preservestrategytype = CurrentStrategyType;
       routeSetOptions(m_options);
    }
    else
    {
        routeSetOptionsAvoidFlag(m_options);
    }

  //  calculateRoute();
  //  close();
    changewindow(ROUTEMAINMENU);
}

void Menu_RouteStrategy ::saveRoutingOptionsSlot()
{
    saveRoutingOptions(m_options);
}
