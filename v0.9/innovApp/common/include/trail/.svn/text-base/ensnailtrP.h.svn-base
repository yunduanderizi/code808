#ifndef ENSNAILTRP_H
#define ENSNAILTRP_H

#include <trail/ensnailtr.h>

#ifdef __cplusplus
extern "C" {
#endif


/// The information we store for a snail trail sample.
typedef struct TSnailTrailSample {
  /// The geographic location of the sample,
  TPos   location;
  /// If set to TRUE, this sample starts a new sample sequence. There is a (data)
  /// gap between this and the previous sample and because of that we shouldn't
  /// interpolate between those two samples.
  BOOL8  gap;
} TSnailTrailSample;


/// Gives the snail trail a new location, which is then used to calculate the
/// snail trail samples in the buffer.
///
/// @param[in]  location
///   The location we would like to record.
/// @param[in]  time
///   The GPS time since midnight in milliseconds this location has been measured.
/// @param[in]  locked
///   Should be TRUE, if the location has a valid map lock.
void snailTrail_recordSample( TPos location, UINT32 time, BOOL8 locked );


/// Returns the number of stored/available samples in the snail trail.
UINT32 snailTrail_getSampleCount(void);

/// Use this function to read a specific sample.
///
/// @param[in]  index
///   The index sample of the sample you want to retrieve.
///   (0 <= index < sampleCount)
/// @return
///   The according sample or NULL, if index is invalid.
const TSnailTrailSample* snailTrail_getSample( UINT32 index );


#ifdef __cplusplus
}
#endif

#endif /* ENSNAILTRP_H */
