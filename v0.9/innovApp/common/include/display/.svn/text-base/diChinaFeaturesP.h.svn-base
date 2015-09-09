#ifndef DICHINAFEATURESP_H
#define DICHINAFEATURESP_H

#include <display/diChinaFeatures.h>
#include <language/lacodepageP.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _tagBINHEAD
{
	UTF8    szIdentity[8];
	UINT32  dwFileLength;
	UINT32  dwObjectCount;
}BINHEAD;

typedef struct _tagBINOBJECT
{
	UINT8   type;
	UINT8   cnSize;
	UINT8   enSize;
	UTF8    szName[61];
	UINT32  dwPtCnt;
}BINOBJECT;


void diChinaFeatures_RenderBoundary_CHN(TViewport *viewport, BINOBJECT& binObj, long* px, long* py);
void diChinaFeatures_RenderProvince_CHN(TViewport *viewport, BINOBJECT& binObj, long* px, long* py, UINT32 pattern);
void diChinaFeatures_RenderComments_CHN(TViewport *viewport, BINOBJECT& binObj, long* px, long* py, int mode);
//void diChinaFeatures_RenderSpecialComments_CHN(TViewport *viewport, BINOBJECT& binObj, long px, long py, , long cx, long cy)

#ifdef __cplusplus
}
#endif

#endif /* DICHINAFEATURESP_H */
