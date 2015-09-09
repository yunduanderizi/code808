#ifndef ENTOKENP_H
#define ENTOKENP_H

#include <containers/gebitset.h>
#include <language/lacodepage.h>
#include <language/lainit.h>
#include <mapdata/enmfileP.h>
#include <mapdata/enmapinfoP.h>
#include <mapdata/entoken.h>

#ifdef __cplusplus
extern "C" {
#endif

// The longest length character sequence that can make up a token part
#define TOKEN_MAX_NAME_SIZE       72

#define TOKEN_MAX_PART_SIZE       4
// The maximum number of a tokens per road name
#define TOKEN_MAX_COUNT           32
//The maximum number of token part per token
#define TOKEN_MAX_PART_COUNT      /*30*/56
//The maximum number of alternate spelling tokens per token
#define TOKEN_MAX_ALT_TOKENS      40
//The maximum number of alternate tokens parts
#define TOKEN_MAX_ALT_TOKEN_PARTS 5

/* The number of bytes per row in the alternate token table */
#define TTOKEN_ALT_TABLE_ROW_SIZE 10

//2 bytes offset in tokenised name every <TOKEN_NAME_MOD> records
#define TOKEN_NAME_MOD_1 8  //for road & place & poitype
#define TOKEN_NAME_MOD_2 4  //only for poi

//The maximum number of characters in the route directional 
#define TOKEN_DIR_STRING_LEN 6

typedef UINT32 TTokenHighlight;

typedef enum {
    TOKEN_DATA_TYPE_ROAD,
    TOKEN_DATA_TYPE_PLACE,
    TOKEN_DATA_TYPE_POI,
    TOKEN_DATA_TYPE_POI_TYPE,
    TOKEN_DATA_TYPES
} TTokenDataType;

typedef enum {
  ROUTE_DIRECTIONAL_UNDEFINED,
  ROUTE_DIRECTIONAL_NORTH = 1,
  ROUTE_DIRECTIONAL_EAST,
  ROUTE_DIRECTIONAL_SOUTH,
  ROUTE_DIRECTIONAL_WEST
} TTokenRouteDir;

typedef enum {
  ALT_SEARCH_TYPE_ANY,        /* Search for All alternate tokens */
  ALT_SEARCH_TYPE_REV,        /* Search for All alternates (in reverse). 
                                 This affects the costing */
  ALT_SEARCH_TYPE_DISP,       /* Search for the display abbreviation */
} TTokenAltSearchType;

/* Object used to return alternate tokens */
typedef struct TTokenAlternate{
  UINT32 tokenId;
  UINT8  subClass;
  UINT8  costClass;
} TTokenAlternate;

typedef struct TTokenCheckData {
  UINT32  tokenId;
  UINT32  tokenPos;
  UINT32  remainCount;
  BOOL8   isPrimaryToken;
} TTokenCheckData;

/* Callbacks for validating tokenised names against tokens */
typedef BOOL8 TTokenBeginNameCallback  ( void   *context,
                                         UINT32  nameId );
typedef BOOL8 TTokenCheckTokenCallback ( void            *context,
                                         TTokenCheckData *check );
typedef BOOL8 TTokenEndNameCallback    ( void   *context );

/* Token Data Source which encapsulates the mapHeader
   and chunk indexes */
typedef struct TTokenDataSource {
  /* The type of this data source */
  TTokenDataType type;

  /* Generic map header */
  TGenericMapInfo *mapInfo;

  /* The 4 character map name, only used for debug */
  char mapName[4];

  /* ID of the first token that is not a separator */
  UINT32 firstNonSeparatorToken;

  /* ID of the first tokenised name that is NOT a route */
  UINT32 firstNonRouteName;

  /* Index of the misc chunk */
  UINT32  miscChunkIdx;

  /* Index of the alt token bitset chunk */
  UINT32  altBitsetIdx;

  /* Index of the alternate token chunk */
  UINT32  altChunkIdx;

  /* Offsets of the alternate tokens tables */
  UINT32  altTableOffet;

  /* Number of rows in the alternates table */
  UINT32  altTableRowCount;

  /* Language Codes */
  UINT8   languageCodes[3];

  /* Since the format of the misc chunk can change between
     POIs and Road files we need to store offsets into the
     various tables stored in this chunk */
  UINT16  miscOffsetMostFrequent;
  UINT16  miscOffsetTokenParts;
  UINT16  miscOffsetExactSpIdx;
  UINT16  miscOffsetPhonemeIdx;
  UINT16  miscOffsetEquivSpell;
  UINT16  miscOffsetDataBlob;

  /* The implicit separator token id */
  UINT16  implicitSepTokenId;

  /* Total number of token parts */
  UINT16  tokenPartCount;

  /* Total number of frequent tokens */
  UINT16  mostFreqTokenCount;
  /* Total number of equivalent spelling token parts */
  UINT16  equivSpellCount;

  /* Constants used to quickly find the matching chunk */
  UINT16 nameChunkConst;
  UINT16 tokenChunkConst;

  /* The index of the first chunk names and the
     number of name chunks */
  UINT32  nameChunkIdx;
  UINT32  nameChunkCount;

  /* The index of the first token chunk and the
     number of token chunks */
  UINT32  tokenChunkIdx;
  UINT32  tokenChunkCount;
} TTokenDataSource;

typedef struct TTokenHeader {
  UINT32          id;                       /* Implicit token ID                                     */
  UINT8           tokenPartCount;           /* Number of token parts                                 */
  BOOL8           isWeak;                   /* Is this a weak token (e.g. 'Road')                    */
  BOOL8           isUsedAsMostSig;          /* Is this token used as the most significant token      */
  BOOL8           isUsedAsNonSignificant;   /* appears in the data as a non most significant token   */
  BOOL8           isUsedInNames;            /* Is this token used in any name                        */
  BOOL8           isConcat;                 /* Is this token concatanatable with other tokens        */
  UINT32          dataOffset;               /* The data offset into the variable section             */
} TTokenHeader;

typedef struct TToken{
  /* Token header */
  TTokenHeader    header;

  /* These fields are not unpacked when token_unpackTokenHeader is called */
  UINT32  tokenParts[TOKEN_MAX_PART_COUNT]; /* The token parts that make up this token*/
} TToken;

typedef struct TTokenName{
  UINT32  id;                              /* Implicit Name ID                             */
  UINT8   tokenCount;                      /* Number of tokens for this name               */
  UINT8   primaryTokenPos;                 /* Index of the most significant token          */
  TLanguageCode languageCode;              /* The Language code                            */
  BOOL8   isRoute;                         /* Is this name a route                         */
  TTokenRouteDir directionalType;          /* Only populated for route names               */
  UTF16   dirString[TOKEN_DIR_STRING_LEN]; /* Directional String                           */
  UINT32  tokens[TOKEN_MAX_COUNT];         /* The token Ids that make up this name         */
  UINT32  sepBits;                         /* Marks all the tokens that are separators     */
  UINT32  adornmentPlaceName;              /* Adornment place name if one exists           */
  UINT16  routeType;                       /* Only populated for route names               */
  UINT32  dataOffset;                      /* Offset of the related variable data section.
                                              This is relative to the current chunk        */
} TTokenName;

typedef struct TTokenNameDetails{
  TLanguageCode  languageCode;                    /* The language code for this name           */
  BOOL8          isRoute;                         /* Is this a route (The following fields are 
                                                     only valid for routes)                    */
  TTokenRouteDir dirType;                         /* The route type                            */
  UTF16          routeType;                       /* Route type                                */
  UTF16          dirString[TOKEN_DIR_STRING_LEN]; /* Route directional string (e.g. "N")       */
} TTokenNameDetails;

typedef struct TTokenPart{
  UINT32  id;                   /* Implicit Token part ID          */
  UINT16  exactSpellingSeqId;   /* Exact Spelling Sequence ID      */
  UINT16  normSpellingSeqId;    /* Normalized Spelling Sequence ID */
  UINT16  phonemeId;            /* Phoneme ID                      */
} TTokenPart;

/* The column to use when extracting token part characters */
typedef enum {
  TOKEN_PART_TYPE_NORM_SP,
  TOKEN_PART_TYPE_EXACT_SP,
  TOKEN_PART_TYPE_PH
} TTokenPartType;

/* Key need to perform binary searches on the alternate tokens */
typedef struct TTokenAltSearchKey{
  UINT32      tokenId;
  UINT8       *tableAddress;
  UINT8       uniqueID;
} TTokenAltSearchKey;

typedef struct TTokenPartSearchKey{
  TTokenPartType          partType;
  const TTokenDataSource  *tokenSource;
  UINT32                  chunkIdx;
  UINT32                  unicodeCharacter;
  UINT32                  checkPos;
} TTokenPartSearchKey;

typedef struct TTokenSearchKey{
  const TTokenDataSource  *tokenSource;
  UINT32                  tokenPartId;
  UINT32                  checkPos;
} TTokenSearchKey;

typedef struct TTokenNameSearchKey{
  const TTokenDataSource  *tokenSource;
  UINT32                  tokenId;
} TTokenNameSearchKey;

//@MRC Start
//Description: for comparing language code 
//             in the table of poi names
typedef struct TTokenNamelLanguageSearchKey{
	const TTokenDataSource  *tokenSource;
	TLanguageCode           languageCode;
} TTokenNameLanguageSearchKey;
//@MRC End

/* BEGIN Cache Objects */
typedef struct TTokenHeaderCacheObj {
  UINT32          tokenId ;
  TGenericMapInfo *mapInfo;
  TTokenDataType  type;
  TTokenHeader    tokenHeader;
}TTokenHeaderCacheObj ;

typedef struct TTokenNameCacheObj {
  UINT32          nameId;
  TGenericMapInfo *mapInfo;
  TTokenDataType  type;
  TTokenName      tokenName;
}TTokenNameCacheObj;

typedef struct TTokenTokenChunkCacheObj {
  TGenericMapInfo *mapInfo;
  TTokenDataType   type;
  UINT32           chunkIdx;
  UINT32           chunkStart;
  UINT32           chunkEnd;
} TTokenTokenChunkCacheObj;

/* END Cache Objects */

/* Compare functions used during binary searches */
extern INT32 tokenCompare(const void *key,UINT32 index);
extern INT32 unsafeTokenPartCompare(const void *key,UINT32 index);

//@MRC Start
//Description: compare language code 
//             in the table of poi names
extern INT32 tokenisedPoiNameLanguageCompare(const void *key,UINT32 index);
//@MRC End

BOOL8 token_init( void );

BOOL8 token_unpackTokenPartChar(
  TTokenPartType           partType,
  const TTokenDataSource  *tokenSource,
  UINT32                   index,
  UINT32                   charPos,
  UTF16                   *output );

BOOL8 token_unsafeUnpackTokenPartChar(
  TTokenPartType          partType,
  const TTokenDataSource *tokenSource,
  UINT32                  chunkIdx,
  UINT32                  index,
  UINT32                  charPos,
  UTF16                  *output );

BOOL8 token_unsafeUnpackTokenPartBlob(
  const TTokenDataSource  *tokenSource,
  UINT32                   blobIndex,
  UINT32                   chunkIdx,
  UTF16                   *output );

BOOL8 token_unpackTokenPartBlob(
  const TTokenDataSource  *tokenSource,
  UINT32                   blobIndex,
  UTF16                   *output );

BOOL8 token_unsafeUnpackTokenPart(
  TTokenPart             *tokenPart,
  const TTokenDataSource *tokenSource,
  INT32                   chunkIdx,
  UINT32                  tokenPartId,
  UTF16                  *tknPartBuff );

BOOL8 token_unpackTokenPart(
  TTokenPart             *tokenPart,
  const TTokenDataSource *tokenSource,
  UINT32                  tokenPartId,
  UTF16                  *tknPartBuff );

BOOL8 token_unpackToken(
  TToken                 *token,
  const TTokenDataSource *tokenSource,
  UINT32                  tokenId,
  UTF16                  *outputBuffer );

BOOL8 token_getAlternateTokenParts(
  UINT32                   tokenPartId,
  const TTokenDataSource  *tokenSource,
  UINT32                  *output,
  UINT8                   *count );

BOOL8 token_unpackNameTokens(
  TTokenName               *tokenisedName,
  const TTokenDataSource   *tokenSource,
  UINT32                    nameId );

BOOL8 token_getRouteInfo(
  const TTokenDataSource   *tokenSource,
  UINT32                    nameId,
  UTF16                    *dirString,
  UINT16                   *routeType);

UINT32 token_getNameCount(
  const TTokenDataSource *tokenSource );

UINT32 token_getTokenCount(
  const TTokenDataSource *tokenSource );

UINT32 token_getTokenPartCount(
  const TTokenDataSource *tokenSource );


INT32 token_compareTokenString(
  UINT32                  token1Id,
  UINT32                  token2Id,
  BOOL8                   ignoreSecondaryDiff,
  const TTokenDataSource *tokenSource);

BOOL8 token_unsafeUnpackTokenNameHeader(
  TTokenName             *roadName,
  const TTokenDataSource *tokenSource,
  UINT32                  chunkIdx,
  UINT32                  miscChunkIdx,
  UINT32                  chunkStart,
  UINT32                  chunkRowCount,
  UINT32                  nameId);

BOOL8 token_getAlternateTokens(
  UINT32                   tokenId,
  const TTokenDataSource  *tokenSource,
  TTokenAlternate         *output,
  UINT8                   *count );

BOOL8 token_getReverseAlternateTokens(
  UINT32                   tokenId,
  const TTokenDataSource  *tokenSource,
  TTokenAlternate         *output,
  UINT8                   *count );

BOOL8 token_getDisplayAbbr(
  UINT32                   tokenId,
  const TTokenDataSource  *tokenSource,
  UINT32                  *output );

INT32 token_compareNameString(
  UINT32                  name1Id,
  UINT32                  name2Id,
  BOOL8                   ignoreSecondaryDiff,
  const TTokenDataSource *tokenSource);

BOOL8 token_getMatchingNameRange(
  UINT32                   tokenId,
  const TTokenDataSource  *tokenSource,
  UINT32                  *nameRangeStart,
  UINT32                  *nameRangeEnd );

BOOL8 token_validateNames(
  const TTokenDataSource   *tokenSource,
  const TBitset            *nameFilterBitset,
  UINT32                    startNameId,
  UINT32                    endNameId,
  void                     *callbackContext,
  TTokenBeginNameCallback  *beginNameCallback,
  TTokenCheckTokenCallback *checkTokenCallback,
  TTokenEndNameCallback    *endNameCallback );

BOOL8 token_isSeparator(
  const TTokenDataSource *tokenSource,
  UINT32 tokenId );

BOOL8 token_unpackTokenHeader(
  TTokenHeader           *tokenHeader,
  const TTokenDataSource *tokenSource,
  UINT32                  tokenId);

BOOL8 token_unsafeUnpackTokenHeader(
  TTokenHeader           *tokenHeader,
  const TTokenDataSource *tokenSource,
  UINT32                  chunkIdx,
  UINT32                  chunkStart,
  UINT32                  chunkRowCount,
  UINT32                  tokenId );

BOOL8 token_unsafeUnpackTokenDetails(
  TToken                 *token,
  const TTokenDataSource *tokenSource,
  UINT32                  chunkIdx,
  UINT32                  chunkStart,
  UINT32                  chunkRowCount,
  UINT32                  tokenId,
  BOOL8                   headerOnly,
  UTF16                  *outputBuffer );

BOOL8 token_unsafeUnpackNameTokens( 
  TTokenName              *tokenisedName,
  const TTokenDataSource  *tokenSource,
  UINT32                   nameId,
  UINT32                   chunkIdx,
  UINT32                   chunkStart,
  UINT32                   chunkRowCount );

BOOL8 token_unpackTokenDetails(
  TToken                 *token,
  const TTokenDataSource *tokenSource,
  UINT32                  tokenId,
  BOOL8                   headerOnly,
  UTF16                  *outputBuffer );

UINT32 token_getNameChunkIndex(
  const TTokenDataSource   *tokenSource,
  UINT32                    nameId,
  UINT32                   *chunkRangeStart,
  UINT32                   *chunkRangeEnd );

UINT32 token_getTokenChunkIndex(
  const TTokenDataSource   *tokenSource,
  UINT32                    tokenId,
  UINT32                   *chunkRangeStart,
  UINT32                   *chunkRangeEnd );

BOOL8 token_getHighlightRanges(
  const TTokenDataSource  *tokenSource,
  UINT32                   nameId,
  BOOL8                    abbreviate,
  UINT32                   matchPositions,
  const TTokenHighlight   *highlights,
  UINT8                   *outTypes,
  UINT8                   *outStarts,
  UINT8                   *outEnds );

UINT8 token_unpackDisplayName(
  const TTokenDataSource  *tokenSource,
  UINT32                   nameId,
  BOOL8                    abbreviate,
  UTF16                   *output );

UINT8 token_unpackIdentityName(
  const TTokenDataSource  *tokenSource,
  UINT32                   nameId,
  UTF16                   *output );

UINT8 token_unpackNameDetails( 
  const TTokenDataSource *tokenSource, 
  UINT32                  nameId, 
  BOOL8                   abbreviate,
  BOOL8                   identityName,
  TTokenNameDetails      *nameDetails,
  UTF16                  *output );

TLanguageCode token_getNameLanguage(
  const TTokenDataSource *tokenSource,
  UINT32                  nameId );

UINT32 token_getAdornmentName(
  const TTokenDataSource *tokenSource,
  UINT32                  nameId );

BOOL8 token_unsafeHasAlternates(
  const TTokenDataSource *tokenSource,
  UINT32                  tokenId ,
  UINT32                  chunkIdx );

BOOL8 token_hasAlternates(
  const TTokenDataSource *tokenSource,
  UINT32                  tokenId );

INT16 token_unpackTokenPartByToken(
  const TTokenDataSource  *tokenSource,
  const UINT32             tokenId,
  UTF16                   *output );

#ifdef __cplusplus
}
#endif

#endif /* ENROADP_H */
