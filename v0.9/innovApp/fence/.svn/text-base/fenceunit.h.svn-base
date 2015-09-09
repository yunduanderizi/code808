#ifndef FENCEUNIT_H
#define FENCEUNIT_H
#include "dbus/gpsinterface/include/gpsinterface.h"
#include <location/locmaplocationP.h>
#include "math/mathmatics.h"
#include "math/enworld.h"
struct FenceStateInfo
{
    int etype;        //电子围栏的类型  0--圆形  1--矩形  2--多边形  3--线路
    int eid;          //电子围栏信息在数据库中的存储id
    int estate;       //电子围栏进入驶出状态  1--进入  0--驶出  2--keep
    int epoint;
    int edeviation;   //当围栏类型为线路时有效，在线路中间段  0--正常进入  1--偏离线路  2--重新返回线路
};

struct FenceStatus
{
    int iInOut;
    int iSegment;
    int iMissed;
};


class Fence
{
public:
    Fence(void);
    ~Fence();

    virtual void addFence(int id) = 0;
    virtual void modifyFence(int id) = 0;
    virtual void removeFence(int id) = 0;
    bool isavoid(UINT32 date,UINT32 utc);
    bool inittimerange(vector<unsigned char> st,vector<unsigned char> ed);
    int m_id;
protected:
    FenceStatus m_status;
    ElectronicFenceData m_fence;
    UINT32 m_startdate;
    UINT32 m_enddate;
    UINT32 m_starttime;
    UINT32 m_endtime;
};

class CircleFence : public Fence
{
public:
    CircleFence(void);
    ~CircleFence();
    void addFence(int id);
    void modifyFence(int id);
    void removeFence(int id);
    bool judgpoint(float x, float y, UINT32 utc, UINT32 date);
    FenceStateInfo getFenceStateInfo(){return m_fenceinfo;}
    void init();
private:
    int m_radius;
    Polyline m_pt;
    FenceStateInfo m_fenceinfo;
};

class RectrangeFence : public Fence
{
public:
    RectrangeFence(void);
    ~RectrangeFence();
    void addFence(int id);
    void modifyFence(int id);
    void removeFence(int id);

    bool judgpoint(float x, float y, UINT32 utc, UINT32 date);
    void initsharp(ElectronicFenceData data);
    FenceStateInfo getFenceStateInfo(){return m_fenceinfo;}
    void init();
private:
    FenceStateInfo m_fenceinfo;
    Polyline m_pt;

    int m_radius;

};

class PathFence : public Fence
{
public:
    PathFence(void);
    ~PathFence();
    void addFence(int id);
    void modifyFence(int id);
    void removeFence(int id);
    bool judgpoint(float x, float y, UINT32 utc, UINT32 date);
    void initsharp(ElectronicFenceData data);
    FenceStateInfo getFenceStateInfo(){return m_fenceinfo;}
    void init();

private:
    FenceStateInfo m_fenceinfo;
    Polyline m_pt;
    int pathcount;
};

class PolygonFence : public Fence
{
public:
    PolygonFence(void);
    ~PolygonFence();
public:
    void addFence(int id);
    void modifyFence(int id);
    void removeFence(int id);
    bool judgpoint(float x, float y, UINT32 utc, UINT32 date);
    FenceStateInfo getFenceStateInfo(){return m_fenceinfo;}
    void init();
private:
    FenceStateInfo m_fenceinfo;
    float m_radius;
    Polyline m_pt;
    int pathcount;
};
#endif // FENCEUNIT_H
