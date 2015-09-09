#ifndef _DILOCAL_H_
#define _DILOCAL_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// assert() can be switched on or off globally here
// for the mapcore library.
// assert() evaluates to nothing in production (release) builds,
// but binds to the C runtime implementation in debug builds.
//
// assert() is essentially an exception mechanism where errors,
// as the name exception indicates, should be reported but
// no attempt should be made to recover from these errors:
// they are truly unexpected errors.
//
#ifndef SIMULATOR
#undef  assert
#define assert(x) ((void)0)
#endif

#include <display/di3Dengi.h>
#include <display/diarc.h>
#include <display/diarcmap.h>
#include <display/diarcsector.h>
#include <display/dibillboard.h>
#include <display/dibuilding.h>
#include <display/dicaricon.h>
#include <display/dicolour.h>
#include <display/didisplayline.h>
#include <display/didisplayname.h>
#include <display/didisplaynameset.h>
#include <display/difavour.h>
#include <display/difonts.h>
#include <display/digeoban.h>
#include <display/digeopen.h>
#include <display/diguide.h>
#include <display/dihighlightlist.h>
#include "diinit.h"
#include <display/dijourney.h>
#include <display/dijourneyleg.h>
#include <display/dilabel.h>
#include <display/dilatlong.h>
#include <display/diline.h>
#include <display/dilod.h>
#include <display/dionewaybuffer.h>
#include <display/dipoi.h>
#include <display/dipoimap.h>
#include <display/dipoiranklist.h>
#include <display/dipoisector.h>
#include <display/dipoisupersector.h>
#include <display/dipoitypelist.h>
#include <display/dipolstyle.h>
#include <display/dipoly.h>
#include <display/dipolygon.h>
#include <display/dipolymap.h>
#include <display/dipolysector.h>
#include <display/dipresentation.h>
#include <display/direct.h>
#include <display/diroadmap.h>
#include <display/diroadnamepos.h>
#include <display/diroadnameposset.h>
#include <display/diroadsector.h>
#include <display/diroadstyle.h>
#include <display/diroadsupersector.h>
#include "disectorarray.h"
#include <display/disegment.h>
#include <display/disettlementnameset.h>
#include <display/disky.h>
#include <display/disnailtr.h>
#include <sound/disound.h>
#include <display/divertex.h>
#include <display/diviewport.h>
#include <display/diviewporttransition.h>
#include <display/diwline.h>
#include <display/diworld.h>
#include <display/diZBuffer.h>
#include <display/textures.h>

#ifdef __cplusplus
}
#endif

#endif
