#ifndef LAGUIDET_H
#define LAGUIDET_H

#include <guidance/gdinstruction.h>
#include <language/latypes.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Text
/// @ingroup Language
/// @{

/// @brief Render an instruction as a short text prompt suitable for displaying
///        on screen.
///
/// This function will take the first instruction on the provided list and turn
/// it into a very brief summarised text instruction.
///
/// @param pInstructions    Pointer to a list of instructions
/// @param instructionCount Number of instructions on the list
/// @param pUTF             Pointer to a string to store the result in
/// @param maxLength        Maximum length of the result string
extern MAPCORE_API void
language_textInstruction (const TLanguageInstruction *pInstructions,
                          const UINT32 instructionCount,
                          UTF16 *pUTF,
                          const UINT32 maxLength);

/// Turn a guidance instruction structure into a language instruction structure
///
/// @param[out] langInstr pointer to the TLanguageInstruction structure
/// @param[in] guideInstr pointer to the TGuideInstruction structure
void language_guideToLanguageInstruction( TLanguageInstruction *langInstr, const TGuideInstruction *guideInstr  );

/// @}

#ifdef __cplusplus
}
#endif

#endif
