#ifndef SNDDATAFILEP_H
#define SNDDATAFILEP_H

#include <sound/snddatafile.h>

#ifdef __cplusplus
extern "C" {
#endif

#if PLATFORM_VORBIS_SUPPORT
#if PLATFORM_VORBIS_FLOATING_POINT
#include <vorbis/vorbisfile.h>
#else
#include <thirdparty/tremor/ivorbisfile.h>
#endif /* PLATFORM_VORBIS_FLOATING_POINT */
#endif /* PLATFORM_VORBIS_SUPPORT */

/// @addtogroup SpeechFile
/// @ingroup Speech
/// @{

#ifdef PLATFORM_BIG_ENDIAN
#error BIG ENDIAN SUPPORT NOT PROVIDED!
#else

/// @internal
/// The packed speech data file header as it exists in the file. The complete
/// structure is 48 bytes long.
typedef STRUCT_PACKED {
  UINT16 version;
  UINT16 language;
  UTF16 name[28]; // NULL terminated text
  UINT8 flagFormat;
  UINT8 flags;
  UINT16 wordCount;
  UINT32 samplesPerSec;
  BITFIELD32 channels:8;
  BITFIELD32 bitsPerSample:16;
  BITFIELD32 supportFlags:8;
  UINT32 compression; // big endian byte order
} TSpeechDataFileHeader;

typedef struct TRiffWaveFileHeader {
  char fileType[4];
  UINT32 fileSize;
  char formatType[4];
  char formatName[4];
  UINT32 formatSize;
  INT16 wFormatTag; 
  UINT16 wChannels; 
  UINT32 dwSamplesPerSec; 
  UINT32 dwAvgBytesPerSec; 
  UINT16 wBlockAlign; 
  UINT16 wBitsPerSample;
  char dataName[4];
  UINT32 dataSize;  
} TRiffWaveFileHeader;


/// Speech data file. Speech files contain voice samples used for the delivery
/// of guidance and traffic instructions and information.
typedef struct {
  /// Handle to the speech data source. When speech data is read from a file,
  /// the datasource is a #OS_FILE.
  void *datasource;
  /// Speech data format, language, gender, and name information.
  TSpeechFileInfo info;
  /// Callback functions to support reading speech data from a source other
  /// than a file.
  TFileCallbacks cb;
  /// @internal
  /// Speech token start positions in blocks. Positions are relative to the
  /// start of the first token, which is always 0.
  INT32 tokenStart[SPEECH_MAX_TOKENS_PER_FILE + 1];
  /// @internal
  /// Offset in bytes marking the start of the speech data in the data file.
  INT32 soundWavesStart;
  /// @internal
  /// Size in bytes of the smallest block of audio data. e.g. for 16-bit
  /// stereo, there are 2 bytes per sample and 2 channels, so the minimum
  /// data size is 4 bytes.
  INT8 bytesPerBlock;
  /// @internal
  /// Format specific file state information
  union {
#if PLATFORM_VORBIS_SUPPORT
    OggVorbis_File vf;
#endif /* PLATFORM_VORBIS_SUPPORT */
    struct {
      INT32 blockNum;
      INT32 sampleNum;
      INT32 fileBlockNum;
    } dvi;

  } state;

} TSpeechFile;

/// @internal
/// Read a 16-bit big endian value from an unaligned memory location
#define /* UINT16 */ READ_U16_BE(/* void * */ addr) \
  ((((UINT8*)addr)[0] << 8) | (((UINT8*)addr)[1]))

/// @internal
/// Read a 16-bit little endian value from an unaligned memory location
#define /* UINT16 */ READ_U16_LE(/* void * */ addr) \
  ((((UINT8*)addr)[0]) | (((UINT8*)addr)[1] << 8))

/// @internal
/// Read a 32-bit big endian value from an unaligned memory location
#define /* UINT32 */ READ_U32_BE(/* void * */ addr) \
  ((((UINT8*)addr)[0] << 24) | \
   (((UINT8*)addr)[1] << 16) | \
   (((UINT8*)addr)[2] << 8) | \
   (((UINT8*)addr)[3]))

/// @internal
/// Read a 32-bit little endian value from an unaligned memory location
#define /* UINT32 */ READ_U32_LE(/* void * */ addr) \
  ((((UINT8*)addr)[0]) | \
   (((UINT8*)addr)[1] << 8) | \
   (((UINT8*)addr)[2] << 16) | \
   (((UINT8*)addr)[3] << 24))

/// @internal
/// Write a 16-bit big endian value to an unaligned memory location
#define /* void */ WRITE_U16_BE(/* void * */ addr, /* UINT16 */ value) \
  (((UINT8*)addr)[0] = (UINT8) (value >> 8), \
   ((UINT8*)addr)[1] = (UINT8) (value))

/// @internal
/// Write a 16-bit little endian value to an unaligned memory location
#define /* void */ WRITE_U16_LE(/* void * */ addr, /* UINT16 */ value) \
  (((UINT8*)addr)[0] = (UINT8) (value), \
   ((UINT8*)addr)[1] = (UINT8) (value >> 8))

/// @internal
/// Write a 32-bit big endian value to an unaligned memory location
#define /* void */ WRITE_U32_BE(/* void * */ addr, /* UINT32 */ value) \
  (((UINT8*)addr)[0] = (UINT8) (value >> 24), \
   ((UINT8*)addr)[1] = (UINT8) (value >> 16), \
   ((UINT8*)addr)[2] = (UINT8) (value >> 8), \
   ((UINT8*)addr)[3] = (UINT8) (value))

/// @internal
/// Write a 32-bit little endian value to an unaligned memory location
#define /* void */ WRITE_U32_LE(/* void * */ addr, /* UINT32 */ value) \
  (((UINT8*)addr)[0] = (UINT8) (value), \
   ((UINT8*)addr)[1] = (UINT8) (value >> 8), \
   ((UINT8*)addr)[2] = (UINT8) (value >> 16), \
   ((UINT8*)addr)[3] = (UINT8) (value >> 24))

#endif /* PLATFORM_BIG_ENDIAN */

/// @}

#ifdef __cplusplus
}
#endif

#endif /* SNDDATAFILE_H */
