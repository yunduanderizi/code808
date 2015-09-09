#ifndef _RGNREG_H
#define _RGNREG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
    Region table format expected by bootloader:

        Total table size : 4 bytes
        
        Region ID count

            Region ID : 4 bytes
            Region level ID : 4 bytes
            Region parent ID (INVALID_REGION if null) : 4 bytes
            Region default description : 129 bytes
            ...

        Region description count : 4 bytes
            
            Region ID : 4 bytes
            Region description : 129 bytes
            Description language ID : 4 bytes
            ...

        Region DCA member count : 4 bytes

            Region ID : 4 bytes
            Member DCA chunk hash : 20 bytes
            ...

        Unlock signature count : 4 bytes

            Unlock signature : 128 bytes
            Unlock signature level : 4 bytes
            ...
       
        Data validation signature (128 bytes)
*/

#define INVALID_REGION          	0xFFFFFFFF

void initBuffer( UINT8 *regionBuffer
               , UINT32 *finalSize 
               );

void addRegion( UINT8  *regionBuffer
              , UINT32 *regionDescriptionStartOffset
              , UINT32 regionID
              , UINT32 regionLevel
              , UINT32 regionParent
              , UINT8 *defaultDescription
              );

void addRegionDescription( UINT8  *regionBuffer
                         , UINT32 *regionDescriptionStartOffset
                         , UINT32 *regionMembershipStartOffset
                         , UINT32 regionID
                         , const  char *regionDesc
                         , UINT32 languageID
                         );

void addRegionMembership( UINT8  *regionBuffer
                        , UINT32 *regionDescriptionStartOffset
                        , UINT32 *regionMembershipStartOffset
                        , UINT32 *signatureOffset
                        , UINT32 regionID
                        , UINT8 *chunkHashMember
                        );

int createSignature( UINT32 iCryptoProvider
                   , UINT32 privateKeyID
                   , UINT8  *unitID
                   , UINT32 unitIDLength
                   , char *unlockString
                   , UINT8 *outputBuffer
                   );

void addSignature( UINT8  *regionBuffer
                 , UINT32 *signatureOffset
                 , UINT32 *finalSize
                 , UINT8 *sigData
                 , UINT32 unlockLevel
                 );

void signImage( UINT32 iCryptoProvider
              , UINT32 privateKeyID
              , UINT8  *regionBuffer
              , UINT32 *finalSize
              );

#ifdef __cplusplus
}
#endif

#endif
