#ifndef ENSEARCHINDEX_H
#define ENSEARCHINDEX_H

#include <language/utf16.h>


#ifdef __cplusplus
extern "C" {
#endif


#define SEARCHINDEX_WHICHBIT_ERROR     (0xFF)

#define SEARCHINDEX_CHUNK_MAPPING_NUM   (5460)  // (16K)
#define SEARCHINDEX_CHUNK_FEATURE_NUM   (16384) // (128K)

#define SIZE_OF_MAPPING (sizeof(TSearchIndexMapping))
#define SIZE_OF_FEATURE (sizeof(TSearchIndexFeature))

#define SEARCHINDEX_CHUN_MAPPING_SIZE   (SEARCHINDEX_CHUNK_MAPPING_NUM * SIZE_OF_MAPPING)
#define SEARCHINDEX_CHUN_FEATURE_SIZE   (SEARCHINDEX_CHUNK_FEATURE_NUM * SIZE_OF_FEATURE)


#pragma pack(1)
typedef struct TSearchIndexMapping {
  UTF16 chineseChar;
  UINT8 whichBit;
} TSearchIndexMapping;

typedef struct TSearchIndexFeature {
  UINT32 low32bit;
  UINT32 high32bit;
} TSearchIndexFeature;
#pragma pack()


#ifdef __cplusplus
}
#endif

#endif /* ENSEARCHINDEX_H */
