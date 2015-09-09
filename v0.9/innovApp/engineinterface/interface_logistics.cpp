#include "interface_logistics.h"

#include "dbus/gpsinterface/include/gpsinterface.h"

void renderMutiDestions(TBitmap*favouriteicon, TViewport* viewport)
{
    if(g_navFavourites == NULL)
    {
        return ;
    }
   g_navFavourites->render(favouriteicon,viewport);
}

void initElectronicFence()
{
#if defined(ARM_LINUX)
    vector<ElectronicFenceData> data;
    getAllElectronicFenceByType(FENCE_TYPE_CIRCLE,data);
#endif
}


