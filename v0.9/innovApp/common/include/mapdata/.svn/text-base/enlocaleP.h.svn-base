#ifndef ENLOCALEP_H
#define ENLOCALEP_H

#include <mapdata/enlocale.h>
#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif

// For now a locale is simply the country code
// in future may want to replace this with a country/state pair.
typedef TNumericISOCountryCode TLocale;

typedef enum {
  LOCALE_EXIT_TYPE_UNKNOWN,
  LOCALE_EXIT_TYPE_EXITS,
  LOCALE_EXIT_TYPE_JUNCTIONS
} TLocaleExitType;

TLocaleExitType locale_getExitType(TLocale locale);

#ifdef __cplusplus
}
#endif

#endif /* ENLOCALEP_H */
