#include "fenceunit.h"
#include "common/include/time/getime.h"


const int daysecond = (24*60*60) * 1000;
const int monthsecond = 30 * daysecond * 1000;
const int yearsecond = 365 * monthsecond * 1000;

Fence::Fence()
{

}

Fence::~Fence()
{

}

bool Fence::isavoid(UINT32 date, UINT32 utc)
{
    return true;
    if(m_startdate == 0)
    {
        if(date < m_startdate) return false;
        if(date > m_enddate) return false;
    }
    else
    {
        if(utc < m_starttime) return false;
        if(utc > m_endtime) return false;
        if(utc < m_starttime) return false;
        if(utc > m_endtime) return false;
    }

    return true;
}

bool Fence::inittimerange(vector<unsigned char> st,vector<unsigned char> ed)
{
    m_startdate = st[0] * yearsecond + st[1] * monthsecond + st[2] * daysecond;
    m_enddate = ed[0] * yearsecond + ed[1] * monthsecond + ed[2] * daysecond;
    m_starttime = (st[3]*3600 + st[4]*60 + st[2]) * 1000;
    m_endtime = (ed[3]*3600 + ed[4]*60 + ed[2]) * 1000;
}

void RectrangeFence::initsharp(ElectronicFenceData data)
{
    double x1,y1,x2,y2;


    if(data.point_count == 2)
    {
        Point pt1 = data.points[0];
        Point pt2 = data.points[1];
        if(pt1.getx() < pt2.getx())
        {
           y1 = pt1.getx();
           y2 = pt2.getx();
           printf("pt1.getx<pt2.getx    y1==%d,y2==%d\n",y1,y2);
        }
        else
        {
            y2 = pt1.getx();
            y1 = pt2.getx();
            printf("pt1.getx>=pt2.getx    y2==%d,y1==%d\n",y2,y1);
        }

        if(pt1.gety() < pt2.gety())
        {
           x1 = pt1.gety();
           x2 = pt2.gety();
           printf("pt1.gety<pt2.gety    x1==%d,x2==%d\n",x1,x2);
        }
        else
        {
            x2 = pt1.gety();
            x1 = pt2.gety();
             printf("pt1.gety>=pt2.gety    x2==%d,x1==%d\n",x2,x1);
        }
//        m_pt.push_back(Location{x1,y1});
//        m_pt.push_back(Location{x2,y1});
//        m_pt.push_back(Location{x2,y2});
//        m_pt.push_back(Location{x1,y2});
        Location loc;
        loc.init(x1 ,y1);
        m_pt.push_back(loc);
        m_pt.push_back(loc);
        m_pt.push_back(loc);
        m_pt.push_back(loc);



    }
}

void RectrangeFence::addFence(int id)
{
#ifdef ARM_LINUX
    struct ElectronicFenceData data;


    int iret = getElectronicFenceByType(FENCE_TYPE_RECTANGLE,id,&data);

    if(iret == -1)
    {
        printf("addFence %d error\n",id);
        return;
    }
    m_pt.clear();
    initsharp(data);
    m_fence = data;
    m_status.iInOut = 0;
    m_status.iSegment = 0;


    inittimerange(data.tm_start,data.tm_stop);

//    printf("addFence %d error\n",data.tm_start);
//            time_stringToUlong(NULL,work)
//    UINT32 m_startdate;
//    UINT32 m_enddate;
//    UINT32 m_starttime;
//    UINT32 m_endtime;


#endif
}

void RectrangeFence::modifyFence(int id)
{
#ifdef ARM_LINUX
    struct ElectronicFenceData data;
    int iret = getElectronicFenceByType(FENCE_TYPE_RECTANGLE,id,&data);
    if(iret == -1)
    {
        printf("modifyFence %d error\n",id);
        return;
    }
    m_pt.clear();
    initsharp(data);
    m_fence = data;
    m_id = id;
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    inittimerange(data.tm_start,data.tm_stop);

#endif
}

void RectrangeFence::removeFence(int id)
{
    //remove fence
    m_pt.clear();
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed == 0;
}

RectrangeFence::RectrangeFence():Fence()
{

}

void RectrangeFence::init()
{
#ifdef ARM_LINUX
    vector<ElectronicFenceData> arrdata;
    int iret = getAllElectronicFenceByType(FENCE_TYPE_RECTANGLE,  arrdata);
    printf("FENCE_TYPE_RECTANGLE %d count %u\n",iret, arrdata.size());
    for(int i = 0; i < arrdata.size();i++)
    {
        addFence(arrdata[i].eid);
    }
#endif
}

RectrangeFence::~RectrangeFence()
{

}

bool RectrangeFence::judgpoint(float x,float y,UINT32 utc,UINT32 date)
{

    if(m_pt.size() == 0)return false;
    if(!isavoid(date,utc)) return false;

    bool bchanged = false;
    int iTmpInout = 0;
    Location loc = {x,y};
    iTmpInout = InPolygon(m_pt,loc);
    if(iTmpInout == 2)
    {
        if(m_status.iInOut == 0)
        {
           m_fenceinfo.estate = 2;
        }
        else if(m_status.iInOut == 1)
        {
           m_status.iInOut = 0;
           m_fenceinfo.eid = m_id;
           m_fenceinfo.epoint = 0;
           m_fenceinfo.estate = 1;
           m_fenceinfo.edeviation = 0;
           m_fenceinfo.etype = FENCE_TYPE_RECTANGLE;
           bchanged = true;

        }
    }
    else if(iTmpInout < 2)
    {
        if(m_status.iInOut == 0)
        {
           m_status.iInOut = 1;
           m_fenceinfo.estate = 1;
           m_fenceinfo.eid = m_id;
           m_fenceinfo.epoint = 0;
           m_fenceinfo.edeviation = 0;
           m_fenceinfo.etype = FENCE_TYPE_RECTANGLE;

           bchanged = true;
        }
        else if(m_status.iInOut == 1)
        {
           m_fenceinfo.estate = 2;
        }
    }
    return bchanged;
}

CircleFence::CircleFence():Fence()
{

}

void CircleFence::init()
{
#ifdef ARM_LINUX
    vector<ElectronicFenceData> arrdata;
    int iret = getAllElectronicFenceByType(FENCE_TYPE_CIRCLE,  arrdata);
    printf("FENCE_TYPE_CIRCLE %d count %u\n",iret, arrdata.size());
    for(int i = 0; i < arrdata.size();i++)
    {
        addFence(arrdata[i].eid);
    }
#endif
}

CircleFence::~CircleFence()
{

}

void CircleFence::addFence(int id)
{
#ifdef ARM_LINUX
    ElectronicFenceData data;
    double x1,y1;
    int iret = getElectronicFenceByType(FENCE_TYPE_CIRCLE,id,&data);

    if(iret == -1)
    {
        printf("FENCE_TYPE_CIRCLE addFence %d error\n",id);
        return;
    }

    m_fence = data;
    m_id = id;
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed == 0;
    m_radius = data.radius;
    Location loc;
    for(int i = 0;i < data.point_count;i++)
    {
        Point pt1 = data.points[i];
        x1 = pt1.gety();
        y1 = pt1.getx();
        loc.init(x1,y1);
        m_pt.push_back(loc);
    }
    inittimerange(data.tm_start,data.tm_stop);
#endif
}

void CircleFence::modifyFence(int id)
{
#ifdef ARM_LINUX
    ElectronicFenceData data;
    double x1,y1;
    int iret = getElectronicFenceByType(FENCE_TYPE_CIRCLE,id,&data);

    if(iret == -1)
    {
        printf("FENCE_TYPE_CIRCLE addFence %d error\n",id);
        return;
    }

    m_fence = data;
    m_id = id;
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed == 0;
    m_radius = data.radius;
    Location loc;
    for(int i = 0;i < data.point_count;i++)
    {
        Point pt1 = data.points[i];
        x1 = pt1.gety();
        y1 = pt1.getx();
        loc.init(x1,y1);
        m_pt.push_back(loc);
    }
    inittimerange(data.tm_start,data.tm_stop);
#endif
}

void CircleFence::removeFence(int id)
{
    m_pt.clear();
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed == 0;
}

bool CircleFence::judgpoint(float x, float y,UINT32 utc,UINT32 date)
{
    bool bchanged = false;
    int iTmpInout = 0;
    Location loc = {x,y};

    if(m_pt.size() == 0)return false;
    if(!isavoid(date,utc)) return false;

    if(m_radius > lineDis(m_pt[0].x,m_pt[0].y,x,y))
    {
        iTmpInout = 0;
    }
    else
    {
        iTmpInout = 2;
    }
     if(iTmpInout == 2)
     {
         if(m_status.iInOut == 0)
         {
            m_fenceinfo.estate = 2;
         }
         else if(m_status.iInOut == 1)
         {
            m_status.iInOut = 0;
            m_fenceinfo.eid = m_id;
            m_fenceinfo.epoint = 0;
            m_fenceinfo.estate = 1;
            m_fenceinfo.edeviation = 0;
            m_fenceinfo.etype = FENCE_TYPE_CIRCLE;
            bchanged = true;

         }
     }
     else if(iTmpInout < 2)
     {
         if(m_status.iInOut == 0)
         {
            m_status.iInOut = 1;
            m_fenceinfo.estate = 0;
            m_fenceinfo.eid = m_id;
            m_fenceinfo.epoint = 0;
            m_fenceinfo.edeviation = 0;
            m_fenceinfo.etype = FENCE_TYPE_CIRCLE;

            bchanged = true;
         }
         else if(m_status.iInOut == 1)
         {
            m_fenceinfo.estate = 2;
         }
     }
     return bchanged;
}

PathFence::PathFence():Fence()
{

}

PathFence::~PathFence()
{

}

void PathFence::init()
{
#ifdef ARM_LINUX
    vector<ElectronicFenceData> arrdata;
    int iret = getAllElectronicFenceByType(FENCE_TYPE_PATH,  arrdata);
    printf("FENCE_TYPE_PATH %d count %u\n",iret, arrdata.size());

    for(int i = 0; i < arrdata.size();i++)
    {
        addFence(arrdata[i].eid);
    }
#endif
}

void PathFence::addFence(int id)
{
#ifdef ARM_LINUX
    ElectronicFenceData data;
    double x1,y1;
    int iret = getElectronicFenceByType(FENCE_TYPE_PATH,id,&data);

    if(iret == -1)
    {
        printf("PathFence addFence %d error\n",id);
        return;
    }

    m_fence = data;
    m_id = id;
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed == 0;

    pathcount = data.point_count - 1;

    Location loc;
    for(int i = pathcount - 1;i >= 0;i--)
    {
        Point pt1 = data.points[i];
        x1 = pt1.gety();
        y1 = pt1.getx() + 0.016;
        loc.init(x1,y1);
        printf("PathFence(x,y) (%f,%f)\n",x1,y1);
        m_pt.push_back(loc);
    }
    inittimerange(data.tm_start,data.tm_stop);
//    for(int i = 0;i < pathcount;i++)
//    {
//        Point pt1 = data.points[i];
//        x1 = pt1.gety();
//        y1 = pt1.getx();
//        printf("PathFence(x,y) (%f,%f)\n",x1,y1);
//        m_pt.push_back(Location{x1,y1});
//    }
#endif
}

void PathFence::modifyFence(int id)
{
#ifdef ARM_LINUX
    double x1,y1;
    struct ElectronicFenceData data;
    int iret = getElectronicFenceByType(FENCE_TYPE_PATH,id,&data);

    if(iret == -1)
    {
        printf("PathFence modifyFence %d error\n",id);
        return;
    }

    m_fence = data;
    m_id = id;
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed = 0;
    pathcount = data.point_count - 1;
    Location loc;
    for(int i = pathcount - 1;i >= 0;i--)
    {
        Point pt1 = data.points[i];
        x1 = pt1.gety();
        y1 = pt1.getx();
        loc.init(x1, y1);
        printf("PathFence(x,y) (%f,%f)\n",x1,y1);
        m_pt.push_back(loc);
    }
    inittimerange(data.tm_start,data.tm_stop);
//    for(int i = 0;i < pathcount;i++)
//    {
//        Point pt1 = data.points[i];
//        x1 = pt1.getym_pt.push_back(Location{x1,y1})();
//        y1 = pt1.getx();

//        m_pt.push_back(Location{x1,y1});
//    }
#endif
}

void PathFence::removeFence(int id)
{
    m_pt.clear();
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed == 0;
}


/*******************************
 *
*******************************/
bool PathFence::judgpoint(float x, float y,UINT32 utc,UINT32 date)
{

    int iTmpInout = 0;
    float length = 0.0;
    int nin = -1;
    Location loc = {x,y};

    if(m_pt.size() == 0)return false;
    if(!isavoid(date,utc)) return false;

    //printf("judgpoint pathcount %d\n",pathcount);
    for(int i = 0 ;i < pathcount;i++)
    {
        length = alSegmentDis(m_pt[i], m_pt[i + 1],loc);
        //in segment

        printf("alSegmentDis length %f\n",length);
        if(length > 0 && length < 0.0002)
        {
            //printf("alSegmentDis length %f\n",length);
            iTmpInout = 1;
            nin = i;
            break;
        }
        //attach last point
        else if(length == 1.0 && i == pathcount - 1)
        {
            iTmpInout = 2;
            nin = pathcount - 1;
            break;
        }
        //on line
        else if(length < 0)
        {
            iTmpInout = 0;
        }
        //out of segment
        else
        {
            iTmpInout = 0;
        }
    }
    if(iTmpInout == 0)
    {
        if(m_status.iInOut == 0 && m_status.iSegment == 0)
        {
            return false;
        }
        //out of segment
        else if(m_status.iInOut == 1)
        {
            m_status.iInOut = 0;
            m_status.iMissed == 1;
            m_fenceinfo.etype = FENCE_TYPE_PATH;
            m_fenceinfo.eid = m_id;
            m_fenceinfo.estate = 2;
            m_fenceinfo.epoint = m_status.iSegment;
            m_fenceinfo.edeviation = 1;
            return true;
        }
        //finished
        else if(m_status.iInOut == 2)
        {
            m_status.iInOut = 0;
            m_status.iSegment = 0;
            m_status.iMissed == 0;
            m_fenceinfo.etype = FENCE_TYPE_PATH;
            m_fenceinfo.eid = m_id;
            m_fenceinfo.estate = 0;
            m_fenceinfo.epoint = pathcount - 1;
            m_fenceinfo.edeviation = 0;
            return true;
        }
        else
        {
            return false;
        }

    }
    else if(iTmpInout == 1)
    {
        //check in segment
        if(m_status.iInOut == 0 && m_status.iSegment == 0 && m_status.iMissed != 1)
        {
            m_status.iInOut = 1;
            m_status.iSegment = nin;
            m_fenceinfo.etype = FENCE_TYPE_PATH;
            m_fenceinfo.eid = m_id;
            m_fenceinfo.estate = 1;
            m_fenceinfo.epoint = nin;
            m_fenceinfo.edeviation = 0;
            return true;
        }
        //return back segment
        else if(m_status.iInOut == 0 && m_status.iMissed == 1)
        {
            m_status.iInOut = 1;
            m_status.iSegment = nin;
            m_status.iMissed = 0;
            Location loc={x,y};

           //m_fenceinfo.etype.push_back(loc);
            m_fenceinfo.etype = FENCE_TYPE_PATH;
            m_fenceinfo.eid = m_id;
            m_fenceinfo.estate = 2;
            m_fenceinfo.epoint = nin;
            m_fenceinfo.edeviation = 1;
            return true;
        }
        //go to next
        else if(m_status.iInOut == 1 && m_status.iSegment+1 == nin && m_status.iMissed == 0)
        {
            m_status.iInOut = 1;
            m_status.iSegment = nin;
            m_status.iMissed = 0;
            m_fenceinfo.etype = FENCE_TYPE_PATH;
            m_fenceinfo.eid = m_id;
            m_fenceinfo.estate = 2;
            m_fenceinfo.epoint = nin;
            m_fenceinfo.edeviation = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
    Location loc1;
    loc1.init(x ,y);
    m_pt.push_back(loc);
    //ready for finish
//    else if(iTmpInout == 2)
//    {
//        m_status.iInOut = 2;
//        return false;
//    }
//    else
//    {
//        return false;
//    }
    return false;
}

void PathFence::initsharp(ElectronicFenceData data)
{

}


PolygonFence::PolygonFence():Fence()
{

}

PolygonFence::~PolygonFence()
{

}

void PolygonFence::init()
{
#ifdef ARM_LINUX
    vector<ElectronicFenceData> arrdata;
    int iret = getAllElectronicFenceByType(FENCE_TYPE_POLYGON,  arrdata);
    printf("FENCE_TYPE_POLYGON %d count %u\n",iret, arrdata.size());
    for(int i = 0; i < arrdata.size();i++)
    {
        addFence(arrdata[i].eid);
    }
#endif
}

void PolygonFence::addFence(int id)
{
#ifdef ARM_LINUX
    double x1,y1;
    struct ElectronicFenceData data;
    int iret = getElectronicFenceByType(FENCE_TYPE_POLYGON,id,&data);

    if(iret == -1)
    {
        printf("PathFence addFence %d error\n",id);
        return;
    }

    m_fence = data;
    m_id = id;
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed = 0;
    pathcount = data.point_count - 1;

    Location loc;
    for(int i = 0;i < pathcount;i++)
    {
        Point pt1 = data.points[i];
        x1 = pt1.gety();
        y1 = pt1.getx();
        loc.init(x1,y1);
        m_pt.push_back(loc);
    }
    inittimerange(data.tm_start,data.tm_stop);
#endif
}

void PolygonFence::modifyFence(int id)
{
#ifdef ARM_LINUX
    double x1,y1;
    struct ElectronicFenceData data;

    int iret = getElectronicFenceByType(FENCE_TYPE_POLYGON,id,&data);

    if(iret == -1)
    {
        printf("PathFence addFence %d error\n",id);
        return;
    }

    m_fence = data;
    m_id = id;
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed = 0;
    pathcount = data.point_count - 1;

    Location loc;

    for(int i = 0;i < pathcount;i++)
    {
        Point pt1 = data.points[i];
        x1 = pt1.gety();
        y1 = pt1.getx();
        loc.init(x1, y1);
        //m_pt.push_back(Location{x1,y1});
        m_pt.push_back(loc);
    }
    inittimerange(data.tm_start,data.tm_stop);
#endif
}

void PolygonFence::removeFence(int id)
{
    m_pt.clear();
    m_status.iInOut = 0;
    m_status.iSegment = 0;
    m_status.iMissed == 0;
}

bool PolygonFence::judgpoint(float x, float y,UINT32 utc,UINT32 date)
{
    bool bchanged = false;

    int iTmpInout = 0;
    Location loc = {x,y};

    if(m_pt.size() == 0)return false;
    if(!isavoid(date,utc)) return false;

    iTmpInout = InPolygon(m_pt,loc);
    if(iTmpInout == 2)
    {
        if(m_status.iInOut == 0)
        {
           m_fenceinfo.estate = 2;
        }
        else if(m_status.iInOut == 1)
        {
           m_status.iInOut = 0;
           m_fenceinfo.eid = m_id;
           m_fenceinfo.epoint = 0;
           m_fenceinfo.estate = 1;
           m_fenceinfo.edeviation = 0;
           m_fenceinfo.etype = FENCE_TYPE_POLYGON;
           bchanged = true;

        }
    }
    else if(iTmpInout < 2)
    {
        if(m_status.iInOut == 0)
        {
           m_status.iInOut = 1;
           m_fenceinfo.estate = 0;
           m_fenceinfo.eid = m_id;
           m_fenceinfo.epoint = 0;
           m_fenceinfo.edeviation = 0;
           m_fenceinfo.etype = FENCE_TYPE_POLYGON;

           bchanged = true;
        }
        else if(m_status.iInOut == 1)
        {
           m_fenceinfo.estate = 2;
        }
    }
    return bchanged;
}
