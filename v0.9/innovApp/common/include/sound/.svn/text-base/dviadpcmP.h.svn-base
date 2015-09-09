#ifndef DVIADPCMP_H
#define DVIADPCMP_H

#include <sound/dviadpcm.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

// RIFF/WAVE header structures. These are in no way specific to DVI ADPCM and
// don't really belong in this header. However, they're not used by any other
// code, so we keep them here for convenience.

#define RIFF_CHUNK_ID     0x52494646 // 'RIFF' 
#define FORMAT_CHUNK_ID   0x666d7420 // 'fmt ' 
#define DATA_CHUNK_ID     0x64617461 // 'data'
#define FACT_CHUNK_ID     0x66616374 // 'fact'

#define RIFF_WAVE_FORMAT  0x57415645 // 'WAVE'  

// These wave format types are defined in the Microsoft Platform SDK header
// mmreg.h. We conditionally define them here to avoid defining them twice.
#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM       0x0001
#endif

#ifndef WAVE_FORMAT_DVI_ADPCM
#define WAVE_FORMAT_DVI_ADPCM 0x0011
#endif

/// The classic "Resource Interchange File Format" header.
typedef struct {
  /// Contains the letters "RIFF" in ASCII form.
  UINT32 chunkID;
  /// Size of the rest of the file - 8 bytes.
  UINT32 chunkSize;
  /// Contains the letters "WAVE" in ASCII form.
  UINT32 format;
} TRIFFChunkHeader;


/// The "fmt" subchunk describes the sound data's format.
typedef struct {
  /// Contains the letters "fmt " in ASCII form.
  UINT32 subChunkID;
  /// Size of the subchunk - 8 bytes. 20 for DVI ADPCM.
  UINT32 subChunkSize;
  /// 0x0011 = DVI ADPCM
  UINT16 formatTag;
  /// 1 for mono, 2 for stereo.
  UINT16 channels;
  /// Frequency of the wave file. Normally 8000, 11025, 22050, or 44100 Hz.
  UINT32 samplesPerSec;
  /// Total average data rate. (samplesPerSec / samplesPerBlock * blockAlign)
  UINT32 avgBytesPerSec;
  /// Block alignment in bytes. Samples must be read from blockAlign boundaries.
  UINT16 blockAlign;
  /// Number of bits per sample of data. DVI ADPCM supports 3 or 4 bits per sample.
  UINT16 bitsPerSample;
} TFmtSubChunkHeader;


/// In DVI ADPCM, the fmt header contains extended information.
typedef struct {
  /// The size in bytes of the extra information in the extended WAVE 'fmt'
  /// header. This should be 2.
  UINT16 extraSize;
  /// Count of the number of samples per channel per Block.
  UINT16 samplesPerBlock;
} TDVIADPCMExtFmtHeader;


/// The "data" subchunk contains the size of the data and the actual sound
typedef struct {
  /// Contains the letters "data" in ASCII form.
  UINT32 subChunkID;
  /// Size of the subchunk - 8 bytes
  UINT32 subChunkSize;
  // The actual sound data
  //UINT8 data[];
} TDataSubChunkHeader;


/// The "fact" chunk is required by all WAVE formats except PCM.
typedef struct {
  /// Contains the letters "fact" in ASCII form.
  UINT32 subChunkID;
  /// Size of the fact chunk - 8 bytes
  UINT32 subChunkSize;
  /// Number of samples in the wave data
  UINT32 numSamples;
} TFactSubChunkHeader;


/// DVI ADPCM block header
typedef struct {
  /// The first sample value of the block. When decoding, this will be used as 
  /// the previous sample to start decoding with.
  INT16 sample0;
  /// The current index into the step table array. (0 - 88)
  UINT8 stepTableIndex;
  /// This byte is reserved for future use.
  UINT8 reserved;
} TDVIADPCMBlockHeader;


/// Count of the number of samples per channel per Block.
/// e.g. 512 byte mono 4-bit ADPCM has 1017 samples per block
#define DVIADPCM_SAMPLES_PER_BLOCK(blockAlign, channels, bitsPerSample) \
  ((((blockAlign - (4 * channels)) * 8) / (bitsPerSample * channels)) + 1)


extern INT32 dviadpcm_blockEncode (INT8 *outBuffer,
                                   const INT16 *inBuffer,
                                   const UINT32 blockSize,
                                   INT32 *index);

extern INT32 dviadpcm_blockDecode (INT16 *outBuffer,
                                   const INT8 *inBuffer,
                                   const UINT32 blockSize);

extern INT32 dviadpcm_findSample(const INT32 sampleNum,
                                 const UINT32 blockSize,
                                 INT32 *blockNum);

#ifdef __cplusplus
}
#endif

#endif /* DVIADPCMP_H */
