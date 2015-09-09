#ifndef ENARFILEP_H
#define ENARFILEP_H

#include <mapdata/enrfile.h>
#include <mapdata/enmfile.h>
#include <mapdata/enroadP.h>
#include <mapdata/enmapinfoP.h>
#include <innovEngine_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

/* version of archive maps that the engine understands */
#define ARCHIVEFILE_MAJOR_VERSION 140
#define ARCHIVEFILE_MINOR_VERSION   1

extern BOOL8          archivefile_init(void);
extern TMapfileStatus archive_load(const char *const mapName);

#ifdef __cplusplus
}
#endif

#endif /* ENARFILEP_H */
