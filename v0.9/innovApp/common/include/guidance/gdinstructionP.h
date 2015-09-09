#ifndef GDINSTRUCTIONP_H
#define GDINSTRUCTIONP_H

#include <guidance/gdinstruction.h>
#include <language/laguides.h>
#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @internal
/// This defines the order in which sign post info strings are packed into the
/// #TGuideInstruction signPostString field.
typedef enum {
  /// @internal
  /// Exit Number.
  GUIDE_STRING_EXIT_NUMBER,
  /// @internal
  /// Route Number.
  GUIDE_STRING_ROUTE_NUMBER,
  /// @internal
  /// Route Directional. [N]orth, [E]ast, [S]outh, or [W]est. These are only
  /// used in North America TA data.
  GUIDE_STRING_ROUTE_DIR,
  /// @internal
  /// Place Name
  GUIDE_STRING_PLACE_NAME,
  /// @internal
  /// Other Destination, e.g. an airport, harbour, or industrial area.
  GUIDE_STRING_OTHER_DEST,
  /// @internal
  /// Street Name
  GUIDE_STRING_STREET_NAME,
  /// @internal
  /// The number of signpost strings.
  GUIDE_NUM_STRINGS
} TGuideStringType;

/// @internal
/// The route number is of SIGN_POST_CONTYP_BRANCH. If we have a branch route
/// number, the instruction may be spoken as "... onto the XXX". Otherwise, it
/// will be spoken as "... and follow signs to the XXX".
#define GD_SIGN_POST_FLAG_BRANCH 0x0001
/// @internal
/// The route number is ambiguous. If the same route number appears on two
/// different signposts with different paths, it is ambiguous. This may mean we
/// decide to leave it out of the spoken instruction.
#define GD_SIGN_POST_FLAG_AMBIGUOUS 0x0002

// Store a number of strings per instruction.
// This number is arbitrary.
#define GD_INSTR_MAX_STRINGS 8

typedef struct {
  /// @internal
  /// A structure describing the signpost information for a given
  /// instruction. The strings buffer of null terminated strings
  /// is described by parallel arrays.
  UINT32 stringIdx[GD_INSTR_MAX_STRINGS]; // index into strings buffer
  TGuideStringType guideStringType[GD_INSTR_MAX_STRINGS]; // type for each str
  TRoadStringID roadStringID[GD_INSTR_MAX_STRINGS]; // descriptor for
                                                    // regenerating phonetics
  INT16 langCode[GD_INSTR_MAX_STRINGS]; // TLanguageCode
  /// @internal
  /// Flags for the signpost strings. These use the GD_SIGN_POST defines.
  UINT16 flags;
  UTF16 strings[1]; // buffer for strings
} TGuideInstrSignPostInfo;

extern BOOL8 guideInstr_getRoadName(const TGuideInstruction *pThis,
                                    UTF16 *name);

extern const UTF16 *guideInstr_getSignPostString(
  const TGuideInstruction *pInstr,
  const TGuideStringType strType);

extern UINT32 guideInstr_getSignPostPhoneticString(
  const TGuideInstruction *pInstr,
  TGuideStringType strType,
  TLanguageCode languageCode,
  UTF16 *outputBuffer);

extern void guideInstr_getSignPostStrs(
  const TGuideInstruction *pInstr,
  TGuideStringType strType,
  TLanguageCode languageCode,
  UTF16 *phoneticString, // PHONE_MAX_SIZE
  UINT32 *phoneticStringLen,
  UTF16 *textString,
  UINT32 *textStringLen);

extern TNodeID guideInstr_getNode(const TGuideInstruction *pThis);

extern TSegmentID guideInstr_getSource(const TGuideInstruction *pThis);

extern TSegmentID guideInstr_getDest(const TGuideInstruction *pThis);

extern INT32 guideInstr_destPathIndex(const TGuideInstruction *pThis);

extern TLanguageStyle guideInstr_getLanguageStyle(const TGuideInstruction *pInstr);

extern TLanguageGrade guideInstr_getLanguageGrade(const TGuideInstruction *pInstr);

extern TLanguageDirection guideInstr_getLanguageDirection(const TGuideInstruction *pInstr);

extern TLanguageFlags guideInstr_getLanguageFlags(const TGuideInstruction *pInstr);

extern TLanguageLanes guideInstr_getLanguageLaneInfo(const TGuideInstruction *pInstr);

extern const char *guideInstr_dirAsString(const TGuideInstruction *pThis);

extern const char *guideInstr_gradeAsString(const TGuideInstruction *pThis);

extern const char *guideInstr_styleAsString(const TGuideInstruction *pThis);

extern const char *guideInstr_priorityAsString(const TGuideInstruction *pThis);

extern BOOL8 guideInstr_isStartOfLeg(const TGuideInstruction *pThis);


#ifdef __cplusplus
}
#endif

#endif /* GDINSTRUCTIONP_H */
