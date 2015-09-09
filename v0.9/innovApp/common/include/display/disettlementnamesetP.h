#ifndef DISETTLEMENTNAMESETP_H
#define DISETTLEMENTNAMESETP_H

#include <display/disettlementnameset.h>
#include <mapdata/enpoiP.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void SettlementNameSet_Prepare(TSettlementNameSet *pThis);
extern void SettlementNameSet_Reserve(TSettlementNameSet *pThis, TDisplayList *displist, struct TViewport *viewport);
extern void SettlementNameSet_Show(TSettlementNameSet *pThis, struct TViewport *viewport);
extern BOOL8 SettlementNameSet_Add(TSettlementNameSet *pThis,
                                   const TPOISettlement *settlement,
                                   TGenericMapInfo *const mapInfo);
extern void SettlementNameSet_Init(TSettlementNameSet *pThis);
extern void SettlementNameSet_RecordIndex(TSettlementNameSet *pThis, INT8 index);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DISETTLEMENTNAMESETP_H */
