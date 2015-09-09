#ifndef ENPHONEP_H
#define ENPHONEP_H

#include <mapdata/enphone.h>
#include <language/lacodepage.h>
#include <mapdata/enmapinfoP.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum length of a single phonetic character part */
#define PHONE_MAX_CHAR_PART_SIZE 20

typedef struct TPhoneticDataSource {
  /* The type of this data source */
  TPhoneticDataType type;

  /* Generic map header */
  TGenericMapInfo *mapInfo;

  /* Index of the dictionary and data blob chunk */
  UINT32 dataChunkIdx;

  /* The chunk index of the first phonetic data chunk */
  UINT32 chunkIndex;

  /* The number of phonetic chunks in the map */
  UINT32 chunkCount;

} TPhoneticDataSource ; 


typedef struct TPhoneticStringIterator {
  /* The nameId we are looking for */
  UINT32 nameId;

  /* Language Code we are looking for */
  TLanguageCode languageCode;

  /* The index of the chunk containing the phonetic string */
  UINT32 chunkIdx;

  /* Offset inside the chunk. */
  UINT32 varOffset;

  /* TRUE if we have reached the last phonetic string */
  BOOL8 lastRow;

  /* TRUE if we have also tried using the phonetic callback */
  BOOL8 callbackUsed;
} TPhoneticStringIterator;


UINT32 phone_getPhoneticString( 
  const TPhoneticDataSource* phoneSource,
  UINT32                     nameId,
  TLanguageCode              languageCode,
  UTF16*                     outputBuffer );

void phone_lockPhoneticIteratorSource(
  const TPhoneticStringIterator *phoneIterator,
  const TPhoneticDataSource* const phoneSource );

void phone_unlockPhoneticIteratorSource(
  const TPhoneticStringIterator *phoneIterator,
  const TPhoneticDataSource* const phoneSource );

BOOL8 phone_initPhoneticStringIterator(
  TPhoneticStringIterator*   phoneticIterator,
  const TPhoneticDataSource* phoneSource,
  TLanguageCode              languageCode,
  UINT32                     nameId );

UINT32 phone_unsafeUnpackNextPhoneticString(
  TPhoneticStringIterator*   phoneIterator,
  const TPhoneticDataSource* const phoneSource,
  TLanguageCode*             outputLangCode,
  UTF16*                     outputBuffer );

#ifdef __cplusplus
}
#endif

#endif /* ENPHONEP_H */
