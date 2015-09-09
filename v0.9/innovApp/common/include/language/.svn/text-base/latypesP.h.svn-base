#ifndef LATYPESP_H
#define LATYPESP_H

#include <language/latypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Language
/// @{

/** Initialise an empty language instruction. A newly initialised instruction
    is spoken as "continue".

    @param[out] instruction Instruction to initialise.

    @return void */
extern MAPCORE_API
void language_initInstruction( TLanguageInstruction *instruction );

/** Get the value of language instruction flags.

    @param[in] instruction Instruction containing the flags to get.

    @param[in] flags The flags to get. To get

    @return TRUE if (all of) the requested flag(s) are enabled, FALSE
    otherwise. */
extern MAPCORE_API
BOOL8 language_getInstructionFlags( const TLanguageInstruction *instruction,
                                    TLanguageFlags flags );

/** Set the value of language instruction flags.

   @param[out] instruction Instruction containing the flags to get.

   @param[in] flags The flags to be set. To set multiple flags, values should
   be OR'ed together. E.g. (LANGUAGE_FLAG_MOTORWAY | LANGUAGE_FLAG_TOLL)

   @param[in] value TRUE to enable the flags, FALSE to disable them.

   @return void */
extern MAPCORE_API
void language_setInstructionFlags( TLanguageInstruction *instruction,
                                   TLanguageFlags flags,
                                   BOOL8 value );

/// @}

#ifdef __cplusplus
}
#endif

#endif // LATYPESP_H
