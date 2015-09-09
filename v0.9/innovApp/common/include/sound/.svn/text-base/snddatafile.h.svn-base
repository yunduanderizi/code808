#ifndef SNDDATAFILE_H
#define SNDDATAFILE_H

/* The 'Tremor' integer-only ogg vorbis decoder is distributed under a BSD
   style license. For platforms that do not wish to comply with the terms of
   the license, the tremor library may be disabled at compile time. The license
   terms are listed in innovEngine/src/thirdparty/tremor/COPYING */
#ifndef PLATFORM_VORBIS_SUPPORT
#define PLATFORM_VORBIS_SUPPORT 1
#endif

#ifndef PLATFORM_VORBIS_FLOATING_POINT
#define PLATFORM_VORBIS_FLOATING_POINT 0
#endif

#include <io/iocallbacks.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Language
/// @{

/// @addtogroup Speech
/// @ingroup Language
/// @{

/// @addtogroup SpeechFile
/// @ingroup Speech
///
/// Speech data files may be used by @ref Guidance and @ref Traffic to deliver
/// spoken instructions and information. Speech files are encoded either as raw
/// PCM or compressed DVI/IMA ADPCM. This API provides an abstraction layer for
/// reading speech file format information and decoding speech samples. It may
/// be useful for implementing the platform function #os_playSpeechPrompt.
///
/// @par Example:
/// @code
/// static TSpeechFileHandle sfh;
///
/// BOOL8 LoadSpeechFile(const char *dataFile) {
///   OS_FILE f;
///   TSpeechFileInfo *pInfo;
///
///   f = os_fopen(dataFile, "rb");
///   if (f == NULL) {
///     return FALSE;
///   }
/// 
///   /* Initialise memory for the handle, we must call speechFile_freeHandle
///      once the handle is no longer needed */
///   if (speechFile_allocHandle(&sfh) != 0 ){
///     return FALSE;
///   }
///
///   if (speechFile_open(sfh, f) != 0) {
///     os_fclose(f);
///     return FALSE;
///   }
///
///   pInfo = speechFile_getInfo(sfh);
///
///   // Set the language and version for traffic and guidance. This ensures
///   // that the token numbers passed to os_playSpeechPrompt match the speech
///   // file contents.
///   language_setLanguageEncoding(pInfo->language);
///
///   return TRUE;
/// }
///
/// void os_playSpeechPrompt(UINT32 tokenList[], UINT32 numTokens,
///                         TSpeechPriority priority) {
///   char buffer[4096];
///   UINT32 i;
///   INT32 start, length, bytesRead, bytesToRead;
///   TSpeechFileInfo *pInfo;
///   OS_FILE f;
///
///   pInfo = speechFile_getInfo(sfh);
///   f = os_fopen("prompt.raw", "wb");
///
///   // dump the decoded speech prompt to a file.
///   for (i = 0; i < numTokens; i++) {
///     start = speechFile_getTokenInfo(sfh, tokenList[i], &length);
///     speechFile_seek(sfh, start);
///
///     // convert length in samples to length in bytes.
///     length = length * pInfo->channels * pInfo->bitsPerSample;
///
///     while (length > 0) {
///       bytesToRead = imath_min(length, 4096);
///       bytesRead = speechFile_read(sfh, &buffer[0], bytesToRead);
///       if (bytesRead <= 0) {
///         break;
///       }
///       // dump raw decoded samples to a file.
///       os_fwrite(&buffer[0], bytesRead, f);
///       length -= bytesRead;
///     }
///   }
/// }
/// @endcode
///
/// @{

/// A set of constants that represent various sound data encoding formats.
enum SpeechFormatTag {
  /// Raw uncompressed PCM
  speechFormatRAW = 0,
  /// DVI ADPCM wave data. "DPCM" in ASCII form
  speechFormatDVI_ADPCM = 0x4450434D,
  /// Ogg Vorbis. "OGGV" in ASCII form
  speechFormatOGG = 0x4F474756
};

/// Speech gender. All current speech data files should be either male or
/// female.
enum SpeechGenderTag {
  /// Gender unknown
  speechGenderUnknown = 0,
  /// Male
  speechGenderMale,
  /// Female
  speechGenderFemale,
  /// Hermaphrodite
  speechGenderHermaphrodite
};

/// Speech data file format and version information. These values are read
/// directly from the speech file header block.
typedef struct {
  /// Display name/description e.g. "English - UK Female"
  UTF16 name[28];
  /// Speech file version
  UINT16 version;
  /// #TLanguageCode speech file language.
  UINT16 language;
  /// #SpeechFormatTag file format. Raw PCM, DVI ADPCM or Ogg Vorbis.
  UINT32 compression;
  /// TRUE if imperial units are supported, FALSE otherwise. All files
  /// support metric units.
  BOOL8 imperialUnits;
  /// #SpeechGenderTag enumerated value. Male or female.
  UINT8 gender;
  /// Total number of speech tokens
  UINT16 tokenCount;
  /// Sampling frequency. 8000, 11025, 16000, 22050, 32000, or 44100 Hz.
  UINT32 samplesPerSec;
  /// 1 for mono, 2 for stereo.
  UINT16 channels;
  /// Number of bits per sample of data. 8-bit or 16-bit.
  UINT16 bitsPerSample;
} TSpeechFileInfo;


typedef struct TSpeechFileImpl* TSpeechFileHandle;

/// The maximum number of speech tokens found in a data file.
#define SPEECH_MAX_TOKENS_PER_FILE 550

extern MAPCORE_API
void speechFile_init( void );

extern MAPCORE_API
INT32 speechFile_allocHandle( TSpeechFileHandle *handle );

extern MAPCORE_API
INT32 speechFile_freeHandle( TSpeechFileHandle *handle );

extern MAPCORE_API
INT32 speechFile_close(TSpeechFileHandle handle);

extern MAPCORE_API
INT32 speechFile_open(TSpeechFileHandle handle, OS_FILE f);

extern MAPCORE_API
INT32 speechFile_openCallbacks(TSpeechFileHandle handle,
                               void *datasource,
                               const TFileCallbacks *cb);

extern MAPCORE_API
INT32 speechFile_test(TSpeechFileHandle handle, OS_FILE f);

extern MAPCORE_API
INT32 speechFile_testCallbacks(TSpeechFileHandle handle,
                               void *datasource,
                               const TFileCallbacks *cb);

extern MAPCORE_API
INT32 speechFile_testOpen(TSpeechFileHandle handle);

extern MAPCORE_API
INT32 speechFile_total(TSpeechFileHandle handle);

extern MAPCORE_API
INT32 speechFile_seek(TSpeechFileHandle handle, INT32 pos);

extern MAPCORE_API
INT32 speechFile_tell(TSpeechFileHandle handle);

extern MAPCORE_API
const TSpeechFileInfo *speechFile_getInfo(TSpeechFileHandle handle);

extern MAPCORE_API
INT32 speechFile_getTokenInfo(TSpeechFileHandle handle,
                              INT32 tokenNum,
                              INT32 *length);

extern MAPCORE_API
INT32 speechFile_read(TSpeechFileHandle handle, void *buffer, INT32 length);

/// @}

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif /* SNDDATAFILE_H */
