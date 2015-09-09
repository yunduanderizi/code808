#ifndef ENSIFILEP_H
#define ENSIFILEP_H

#include <mapdata/ensifile.h>
#include <mapdata/enmfile.h>
#include <mapdata/ensearchindexP.h>
#include <mapdata/enmapinfoP.h>
#include <innovEngine_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

#define SEARCHINDEXFILE_MAJOR_VERSION 140
#define SEARCHINDEXFILE_MINOR_VERSION   9

/*----------------------------------------------------------------*/

TMapfileStatus searchindex_load(TGenericMapInfo *const mapInfo);

/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* ENSIFILEP_H */
