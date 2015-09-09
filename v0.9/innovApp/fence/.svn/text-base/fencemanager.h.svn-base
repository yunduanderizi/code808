#ifndef FENCEMANAGER_H
#define FENCEMANAGER_H



#include <map>


#include "fence/fenceunit.h"

#define MAXFENCE 16

class FenceManager
{
public:
    FenceManager(void);
    void init();
    virtual ~FenceManager(void);
#if defined(ARM_LINUX)

    FenceStateInfo getRectFenceInfo(int i){return m_arrrectfence[i].getFenceStateInfo();}
    FenceStateInfo getPathFenceInfo(int i){return m_arrpathfence[i].getFenceStateInfo();}
    FenceStateInfo getCircleFenceInfo(int i){return m_arrcirclefence[i].getFenceStateInfo();}
    FenceStateInfo getPolygonFenceInfo(int i){return m_arrpolygonfence[i].getFenceStateInfo();}

    void addFence(int type, int id);
    void modifyFence(int type, int id);
    void deleteFence(int type, int id);

    void receivenotify(int eflag, int eid, int etype, int estate);

    void updateallFence(int type);
    void deleteallFence(int type);

    bool checkpatharr(int i, float x, float y, INT32 nhead, UINT32 utc, UINT32 date);
    bool checkrectarr(int i,float x, float y,UINT32 utc,UINT32 date);
    bool checkcirclearr(int i,float x, float y,UINT32 utc,UINT32 date);
    bool checkpolygonarr(int i,float x, float y,UINT32 utc,UINT32 date);
private:

    vector<PathFence> m_arrpathfence;
    vector<CircleFence> m_arrcirclefence;
    vector<RectrangeFence> m_arrrectfence;
    vector<PolygonFence> m_arrpolygonfence;
    void initcirclefence();
    void initpathfence();
    void initpolygonfence();
    void initrectfence();

    void removeallcirclefence();
    void removeallpathfence();
    void removeallpolygonfence();
    void removeallrectfence();

    void removeonecirclefence(int id);
    void removeonepathfence(int id);
    void removeonepolygonfence(int id);
    void removeonerectfence(int id);

    EnumFenceType getfencetype(int m_type);
#endif


};

#endif // FENCEMANAGER_H
