#include "fencemanager.h"
#include <QPainter>
#if defined(ARM_LINUX)
#include "time.h"
#endif

#if defined(ARM_LINUX)//Arm
FenceManager::FenceManager()
{

}

EnumFenceType FenceManager::getfencetype(int m_type)
{
    EnumFenceType type = FENCE_TYPE_BUTT;
    if(m_type == 1)
            type = FENCE_TYPE_CIRCLE;
    else if(m_type == 2)
            type = FENCE_TYPE_RECTANGLE;
    else if(m_type == 3)
            type = FENCE_TYPE_POLYGON;
    else if(m_type == 4)
            type = FENCE_TYPE_PATH;
    return type;
}


void FenceManager::receivenotify(int eflag, int eid, int etype, int estate)
{
    printf("receivenotify eflag %d, eid %d, etype %d, estate %d.\n",eflag, eid, etype, estate);
    if(estate == 0)
    {
        if(eflag == 0)
        {
            //add one fence
            addFence(etype,eid);
        }
        else if(eflag == 1)
        {
            //modify one fence
            modifyFence(etype,eid);
        }
        else if(eflag == -1)
        {
            //delete one fence
            deleteFence(etype,eid);
        }
    }
    else if(estate == -1)
    {
        deleteallFence(etype);
    }
    else if(estate == 1)
    {
        updateallFence(etype);
    }

}

void FenceManager::init()
{
    initcirclefence();
    initpathfence();
    initpolygonfence();
    initrectfence();
}

void FenceManager::initpathfence()
{
#ifdef ARM_LINUX
    vector<ElectronicFenceData> arrdata;
    int iret = getAllElectronicFenceByType(FENCE_TYPE_PATH,  arrdata);
    printf("FENCE_TYPE_PATH %d count %u\n",iret, arrdata.size());
    int fencecount = arrdata.size();
    if(fencecount > MAXFENCE)
    {
        fencecount = MAXFENCE;
        printf("Warning FENCE_TYPE_PATH count %u too much\n", arrdata.size());
    }
    for(int i = 0; i < fencecount;i++)
    {
        PathFence fence;
        fence.addFence(arrdata[i].eid);
        m_arrpathfence.push_back(fence);
        //m_arrpathfence[i].addFence(arrdata[i].eid);
    }
#endif
}

void FenceManager::initrectfence()
{
#ifdef ARM_LINUX
    vector<ElectronicFenceData> arrdata;
    int iret = getAllElectronicFenceByType(FENCE_TYPE_RECTANGLE,  arrdata);
    printf("FENCE_TYPE_RECTANGLE %d count %u\n",iret, arrdata.size());

    int fencecount = arrdata.size();

    for(int i = 0; i < fencecount;i++)
    {
        RectrangeFence fence;
        fence.addFence(arrdata[i].eid);
        m_arrrectfence.push_back(fence);
    }
#endif
}

void FenceManager::removeallcirclefence()
{
    m_arrcirclefence.clear();
}

void FenceManager::removeallpathfence()
{
    m_arrpathfence.clear();
}

void FenceManager::removeallpolygonfence()
{
    m_arrpolygonfence.clear();
}

void FenceManager::removeallrectfence()
{
    m_arrrectfence.clear();
}

void FenceManager::removeonecirclefence(int id)
{
    for(vector<CircleFence>::iterator vi= m_arrcirclefence.begin();vi!=m_arrcirclefence.end();)
    {
        if((*vi).m_id == id)
        {
           m_arrcirclefence.erase(vi);
           return;
        }
        ++vi;
    }
}

void FenceManager::removeonepathfence(int id)
{
    for(vector<PathFence>::iterator vi= m_arrpathfence.begin();vi!=m_arrpathfence.end();)
    {
        if((*vi).m_id == id)
        {
           m_arrpathfence.erase(vi);
           return;
        }
        ++vi;
    }
}

void FenceManager::removeonepolygonfence(int id)
{
    for(vector<PolygonFence>::iterator vi= m_arrpolygonfence.begin();vi!=m_arrpolygonfence.end();)
    {
        if((*vi).m_id == id)
        {
           m_arrpolygonfence.erase(vi);
           return;
        }
        ++vi;
    }
}

void FenceManager::removeonerectfence(int id)
{
    for(vector<RectrangeFence>::iterator vi= m_arrrectfence.begin();vi!=m_arrrectfence.end();)
    {
        if((*vi).m_id == id)
        {
           m_arrrectfence.erase(vi);
           return;
        }
        ++vi;
    }
}

void FenceManager::initpolygonfence()
{
#ifdef ARM_LINUX
    vector<ElectronicFenceData> arrdata;
    int iret = getAllElectronicFenceByType(FENCE_TYPE_POLYGON,  arrdata);
    printf("FENCE_TYPE_POLYGON %d count %u\n",iret, arrdata.size());

    int fencecount = arrdata.size();
    if(fencecount > MAXFENCE)
    {
        fencecount = MAXFENCE;
        printf("Warning FENCE_TYPE_POLYGON count %u too much\n", arrdata.size());
    }

    for(int i = 0; i < fencecount;i++)
    {
        PolygonFence fence;
        fence.addFence(arrdata[i].eid);
        m_arrpolygonfence.push_back(fence);
    }
#endif
}

void FenceManager::initcirclefence()
{
#ifdef ARM_LINUX
    vector<ElectronicFenceData> arrdata;
    int iret = getAllElectronicFenceByType(FENCE_TYPE_CIRCLE,  arrdata);
    printf("FENCE_TYPE_CIRCLE %d count %u\n",iret, arrdata.size());

    int fencecount = arrdata.size();
    if(fencecount > MAXFENCE)
    {
        fencecount = MAXFENCE;
        printf("Warning FENCE_TYPE_CIRCLE count %u too much\n", arrdata.size());
    }

    for(int i = 0; i < fencecount;i++)
    {
        CircleFence fence;
        fence.addFence(arrdata[i].eid);
        m_arrcirclefence.push_back(fence);
    }
#endif
}

void FenceManager::addFence(int type,int id)
{
    //add one fence to vectors
    struct ElectronicFenceData data;
    EnumFenceType enumtype = FENCE_TYPE_BUTT;
    enumtype = getfencetype(type);
    switch(enumtype)
    {
        case FENCE_TYPE_CIRCLE:
        {
            CircleFence fence;
            fence.addFence(id);
            m_arrcirclefence.push_back(fence);
        }
        break;
        case FENCE_TYPE_RECTANGLE:
        {
            RectrangeFence fence;
            fence.addFence(id);
            m_arrrectfence.push_back(fence);
        }
        break;
        case FENCE_TYPE_POLYGON:
        {
            PolygonFence fence;
            fence.addFence(id);
            m_arrpolygonfence.push_back(fence);
        }
        break;
        case FENCE_TYPE_PATH:
        {
            PathFence fence;
            fence.addFence(id);
            m_arrpathfence.push_back(fence);
        }
        break;
    default:
        break;
    }

}

void FenceManager::modifyFence(int type, int id)
{
    struct ElectronicFenceData data;
    EnumFenceType enumtype = FENCE_TYPE_BUTT;
    enumtype = getfencetype(type);
    switch(enumtype)
    {
        case FENCE_TYPE_CIRCLE:
        {
            removeonecirclefence(id);
            CircleFence fence;
            fence.addFence(id);
            m_arrcirclefence.push_back(fence);
        }
        break;
        case FENCE_TYPE_RECTANGLE:
        {
            removeonerectfence(id);
            RectrangeFence fence;
            fence.addFence(id);
            m_arrrectfence.push_back(fence);
        }
        break;
        case FENCE_TYPE_POLYGON:
        {
            removeonepolygonfence(id);
            PolygonFence fence;
            fence.addFence(id);
            m_arrpolygonfence.push_back(fence);
        }
        break;
        case FENCE_TYPE_PATH:
        {
            removeonepathfence(id);
            PathFence fence;
            fence.addFence(id);
            m_arrpathfence.push_back(fence);
        }
        break;
    default:
        break;
    }
}

void FenceManager::deleteFence(int type, int id)
{
    struct ElectronicFenceData data;
    EnumFenceType enumtype = FENCE_TYPE_BUTT;
    enumtype = getfencetype(type);
    switch(enumtype)
    {
        case FENCE_TYPE_CIRCLE:
        {
            removeonecirclefence(id);
        }
        break;
        case FENCE_TYPE_RECTANGLE:
        {
            removeonerectfence(id);
        }
        break;
        case FENCE_TYPE_POLYGON:
        {
            removeonepolygonfence(id);
        }
        break;
        case FENCE_TYPE_PATH:
        {
            removeonepathfence(id);
        }
        break;
    default:
        break;
    }
}

void FenceManager::updateallFence(int type)
{
    //modify one fence from vectors
    EnumFenceType enumtype = FENCE_TYPE_BUTT;
    enumtype = getfencetype(type);
    switch(enumtype)
    {
        case FENCE_TYPE_CIRCLE:
        removeallcirclefence();
        initcirclefence();
        break;
        case FENCE_TYPE_RECTANGLE:
        removeallrectfence();
        initrectfence();
        break;
        case FENCE_TYPE_POLYGON:
        removeallpolygonfence();
        initpolygonfence();
        break;
        case FENCE_TYPE_PATH:
        removeallpathfence();
        initpathfence();
        break;
        default:
            break;
    }
}

void FenceManager::deleteallFence(int type)
{
    //remove one fence from vectors
    struct ElectronicFenceData data;
    EnumFenceType enumtype = FENCE_TYPE_BUTT;
    enumtype = getfencetype(type);
    switch(enumtype)
    {
        case FENCE_TYPE_CIRCLE:
        removeallcirclefence();
        break;
        case FENCE_TYPE_RECTANGLE:
        removeallrectfence();
        break;
        case FENCE_TYPE_POLYGON:
        removeallpolygonfence();
        break;
        case FENCE_TYPE_PATH:
        removeallpathfence();
        break;
        default:
            break;
    }
}

bool FenceManager::checkrectarr(int i,float x,float y,UINT32 utc,UINT32 date)
{
     if(i >= m_arrrectfence.size()) return false;
     return m_arrrectfence[i].judgpoint(x,y,utc,date);
}

bool FenceManager::checkpatharr(int i,float x,float y,INT32 nhead,UINT32 utc,UINT32 date)
{
    if(i >= m_arrpathfence.size()) return false;
    return m_arrpathfence[i].judgpoint(x,y,utc,date);
}

bool FenceManager::checkcirclearr(int i,float x,float y,UINT32 utc,UINT32 date)
{
    if(i >= m_arrcirclefence.size()) return false;
    return m_arrcirclefence[i].judgpoint(x,y,utc,date);
}

bool FenceManager::checkpolygonarr(int i,float x,float y,UINT32 utc,UINT32 date)
{
    if(i >= m_arrpolygonfence.size()) return false;
    return m_arrpolygonfence[i].judgpoint(x,y,utc,date);
}

FenceManager::~FenceManager(void)
{
}



#else//Linux
FenceManager::FenceManager()
{

}
FenceManager::~FenceManager(void)
{
}

#endif
