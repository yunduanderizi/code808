#ifndef ROUTESTRATEGYMENU_H
#define ROUTESTRATEGYMENU_H

#include "navibasewnd.h"
#include "ui_menu_routeStrategy070100.h"
#include "engineinterface/engineinterface.h"

typedef enum RouteStrategy
{
    STRATEGY_SHORTEST=0,
    STRATEGY_FASTEST,
    STRATEGY_EASIEST,
    STRATEGY_ENCONOMICAL
}RouteStrategy;

namespace Ui
{
class routestrategymenu;
}

class Menu_RouteStrategy : public NaviBaseWnd
{
    Q_OBJECT
public:
    Menu_RouteStrategy(QWidget *parent = NULL);
    void setOptions(TRoutingOptions &m_options);
    void calculateRoute();

signals:

public slots:
    void strategyLeftChange();
    void strategyRightChange();
    void backSlot();
    void setRecommendSlot();
    void setShortestSlot();
    void setFastestSlot();
    void setEasiestSlot();
    void setEconomicalSlot();
    void setExpresswaySlot();
    void updateAvoidTollsSlot();
    void updateAvoidUnsealedSlot();
    void updateAvoidFerriesSlot();
    void updateAvoidUturnsSlot();
    void updateAvoidFreewaySlot();
    void saveRoutingOptionsSlot();
    void initOldCheckFlag();
private :
    TRoutingOptions m_options;
    void setRecommend(TRoutingOptions &m_options);
    void setShortest(TRoutingOptions &m_options);
    void setFastest(TRoutingOptions &m_options);
    void setEasiest(TRoutingOptions &m_options);
    void setEconomical(TRoutingOptions &m_options);
    void setExpressway(TRoutingOptions &m_options);
    void saveRoutingOptions(TRoutingOptions &m_options);
    void setStrategyText(enum RouteStrategy type);
    void setStrategy(enum RouteStrategy strategytype);

    void setEasiestAvoid();
    void setEconomicalAvoid();
    void setShortestAvoid();
    void setFastestAvoid();

    BOOL8  oldavoidTollsFlag ;
    BOOL8  oldavoidUnsealedFlag ;
    BOOL8  oldavoidFerriesFlag ;
    BOOL8  oldavoidUturnsFlag ;
    BOOL8  oldavoidFreewayFlag ;
    BOOL8  strategyChanged;

private:
    Ui::routestrategymenu ui;
};


#endif // ROUTESTRATEGYMENU_H
