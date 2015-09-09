#include <math/enworld.h>
#include <math/gemath.h>
#include <stdlib.h>

/* generated automatically by makelu */
#define imath_abs(x) ((x) < 0 ? (-(x)) : (x))
#define imath_max(x,y) ((x) > (y) ? (x) : (y))

static INT32 const lookup_forwardScale[138] = {
  4559,
  4558,
  4557,
  4556,
  4554,
  4551,
  4548,
  4544,
  4539,
  4534,
  4529,
  4522,
  4516,
  4508,
  4500,
  4492,
  4482,
  4473,
  4462,
  4451,
  4440,
  4428,
  4415,
  4402,
  4388,
  4373,
  4358,
  4343,
  4327,
  4310,
  4293,
  4275,
  4257,
  4238,
  4218,
  4198,
  4177,
  4156,
  4135,
  4112,
  4090,
  4066,
  4042,
  4018,
  3993,
  3968,
  3942,
  3915,
  3888,
  3861,
  3833,
  3805,
  3776,
  3746,
  3716,
  3686,
  3655,
  3623,
  3592,
  3559,
  3526,
  3493,
  3459,
  3425,
  3391,
  3355,
  3320,
  3284,
  3248,
  3211,
  3174,
  3136,
  3098,
  3059,
  3021,
  2981,
  2942,
  2902,
  2861,
  2820,
  2779,
  2738,
  2696,
  2654,
  2611,
  2568,
  2525,
  2481,
  2438,
  2393,
  2349,
  2304,
  2259,
  2213,
  2168,
  2122,
  2075,
  2029,
  1982,
  1935,
  1887,
  1840,
  1792,
  1744,
  1696,
  1647,
  1598,
  1550,
  1500,
  1451,
  1402,
  1352,
  1302,
  1252,
  1202,
  1151,
  1101,
  1050,
  999,
  948,
  897,
  846,
  795,
  743,
  692,
  640,
  589,
  537,
  485,
  433,
  381,
  329,
  277,
  225,
  173,
  121,
  69,
  17
};

static INT32 const lookup_backwardScale[138] = {
  3680,
  3680,
  3680,
  3682,
  3683,
  3686,
  3688,
  3691,
  3695,
  3699,
  3704,
  3709,
  3714,
  3721,
  3727,
  3734,
  3742,
  3750,
  3759,
  3768,
  3778,
  3788,
  3799,
  3811,
  3823,
  3835,
  3848,
  3862,
  3877,
  3892,
  3907,
  3924,
  3941,
  3958,
  3976,
  3995,
  4015,
  4036,
  4057,
  4079,
  4101,
  4125,
  4149,
  4174,
  4200,
  4227,
  4255,
  4284,
  4314,
  4344,
  4376,
  4409,
  4443,
  4477,
  4514,
  4551,
  4589,
  4629,
  4670,
  4713,
  4756,
  4802,
  4849,
  4897,
  4947,
  4999,
  5052,
  5107,
  5165,
  5224,
  5285,
  5349,
  5414,
  5482,
  5553,
  5626,
  5702,
  5780,
  5862,
  5947,
  6035,
  6126,
  6221,
  6320,
  6423,
  6531,
  6643,
  6759,
  6881,
  7008,
  7141,
  7280,
  7425,
  7578,
  7738,
  7905,
  8082,
  8267,
  8463,
  8668,
  8886,
  9116,
  9359,
  9617,
  9890,
  10182,
  10492,
  10823,
  11178,
  11558,
  11966,
  12406,
  12881,
  13396,
  13955,
  14566,
  15234,
  15969,
  16781,
  17682,
  18687,
  19817,
  21096,
  22553,
  24231,
  26181,
  28478,
  31220,
  34552,
  38685,
  43948,
  50877,
  60409,
  74348,
  96664,
  138150,
  242074,
  977600
};

/**
 * This function can be used to convert from map coordinates
 * to metre as follows:
 *    scale = lookup_forwardScale[abs(latitude) >> 16];
 *    metre = (latitude * scale) >> 12;
 *
 * The scaling from latitude values to metre alters with latitude - at
 * higher latitudes the scale factor decreases.  It is constant for
 * longitudes (use the value at latitude 0).
 *
 * @param x latitude in map coordinate of center of projection.
 *
 * @return the scaling factor from map coordinates to metre
 *         in 20.12 fixed point format
 */
INT32 World_GetForwardScale(UINT8 x)
{
  assert(x < 138);
  return lookup_forwardScale[x];
}

/**
 * This function can be used to convert from metre to
 * map coordinates as follows:
 *    scale = lookup_backwardScale[abs(latitude) >> 16];
 *    latitude = (metre * scale) >> 12
 *
 * The scaling from metre to latitude alters with latitude - at
 * higher latitudes the scale factor increases.  It is constant for
 * longitudes (use the value at latitude 0).
 *
 * @param x latitude in map coordinate of center of projection.
 *
 * @return the scaling factor from metre to map coordinates
 *         in 20.12 fixed point format
 */
INT32 World_GetBackwardScale(UINT8 x)
{
  assert(x < 138);
  return lookup_backwardScale[x];
}



/// Calculate an offset in metres between
/// two points,
/// Scaled at a given latitude
///
/// @param[in] pos1
/// @param[in] pos2
/// @param[in] latitude
///            The latitude affects the scaling of lat/lon to metres.
TPos World_mapToMetres(const TPos *pos1,
                       const TPos *pos2,
                       const INT32 latitude)
{
  INT32 scaleX, scaleY;         // map to metre rescale factors
  TPos offset_m;

  scaleX = World_GetForwardScale(imath_abs(latitude) >> 16);
  scaleY = World_GetForwardScale(0);

  offset_m.x = (INT32)(((INT64)pos1->x - pos2->x) * scaleX) >> 12;
  offset_m.y = (INT32)(((INT64)pos1->y - pos2->y) * scaleY) >> 12;  

  return offset_m;
}

/// Calculate an offset in lat/lon from
/// a given offset in metres.
/// Scaled at a given latitude
///
/// @param[in] pos1_m
///            An offset in metres.
/// @param[in] latitude
///            The latitude affects the scaling of lat/lon to metres.
TPos World_metresToMap(const TPos *pos1_m,
                       const INT32 latitude)
{
  INT32 xbscale, ybscale;         // metre to map rescale factors
  TPos offset;

  xbscale = World_GetBackwardScale(imath_abs(latitude) >> 16);
  ybscale = World_GetBackwardScale(0);

  // determine the scaled width and height
  offset.x = (UINT32)(((UINT64)pos1_m->y * (UINT64)xbscale) >> 12);
  offset.y = (UINT32)(((UINT64)pos1_m->y * (UINT64)ybscale) >> 12);

  return offset;
}

/**
 * Calculate the distance in metre between a point and a given 
 * rectangle, both specified in map units.
 *
 * The center of the given point and the closest point on the 
 * rectangle is used as center of projection to project lat/long
 * to meters
 *
 * @param[in] pos  point in map units
 * @param[in] rect rectangle in map units
 * @return distance in metre between pos and rect
 */
UINT32 World_distanceToRect( const TPos *pos, const TRect *rect )
{
  TPos close; // closest point on the rectangle
  
  //      A   |       B      |   C
  //          |              |
  //   -------O--------------O-------
  //          |              |
  //      D   |       E      |   F
  //          |              |
  //   -------O--------------O-------
  //          |              |
  //      G   |       H      |   I

  if( pos->x < rect->xmin ) {
    // case A, D or G
    if( pos->y < rect->ymin ) {
      // case G -- closest point is bottom left
      close.x = rect->xmin; close.y = rect->ymin;
    }
    else if( pos->y > rect->ymax ) {
      // case A -- closest point is top left
      close.x = rect->xmin; close.y = rect->ymax;
    }
    else {
      // case D -- closest point is on left hand side
      close.x = rect->xmin; close.y = pos->y;
    }
  }
  else if( pos->x > rect->xmax ) {
    // case C, F or I
    if( pos->y < rect->ymin ) {
      // case I -- closest point is bottom right
      close.x = rect->xmax; close.y = rect->ymin;
    }
    else if( pos->y > rect->ymax ) {
      // case C -- closest point is top right
      close.x = rect->xmax; close.y = rect->ymax;
    }
    else {
      // case F -- closest point is on right hand side
      close.x = rect->xmax; close.y = pos->y;
    }
  }
  else {
    // case B, E or H
    if( pos->y < rect->ymin ) {
      // case H -- closest point is on the bottom side
      close.x = pos->x; close.y = rect->ymin;
    }
    else if( pos->y > rect->ymax ) {
      // case B -- closest point is on the top side
      close.x = pos->x; close.y = rect->ymax;
    }
    else {
      // case E -- we are in the box
      return 0;
    }
  }

  return World_distance( &close, pos );


}


/**
 * Calculate the distance in metre between 2 input points specified
 * in map units.
 *
 * The center of the 2 points is used as center of projection
 * to project lat/long to metre
 *
 * @param pos1[input] first input point in map units
 * @param pos2[input] second input point in map units
 * @return distance in metre between pos1 and pos2
 */
UINT32 World_distance(const TPos *pos1, const TPos *pos2)
{
  INT32 centerY;                // center of projection in map units
  INT32 scaleX, scaleY;         // map to metre rescale factors
  INT64 dx_m, dy_m;

  centerY = (pos1->y + pos2->y) / 2;

  scaleX = World_GetForwardScale(abs(centerY) >> 16);
  scaleY = World_GetForwardScale(0);

  dx_m = (((INT64)pos1->x - pos2->x) * scaleX) >> 12;
  dy_m = (((INT64)pos1->y - pos2->y) * scaleY) >> 12;

  return imath_sqrt64((dx_m * dx_m) + (dy_m * dy_m));
}

/// Make an approximation of length based
/// on the difference between two points in a lat/lon
/// co-ordinates. It is not a very accurate
/// approximation but is fast and requires no divisions
/// or square roots.
///
/// @param[in] p1
///            A lat/lon 
/// @param[in] p2
///            A lat/lon
UINT32 approximateDistance(const TPos p1,
                           const TPos p2)
{
  INT32 scaleX, scaleY;
  INT32 dx, dy; // lat/lon values
  INT32 dx_m, dy_m; // metres
  INT32 predictedDist;
  INT32 latitude;

  dx = abs(p1.x - p2.x);
  dy = abs(p1.y - p2.y);

  latitude = p1.y;
  scaleX = World_GetForwardScale(abs(latitude) >> 16);
  scaleY = World_GetForwardScale(0);
  
  dx_m = (INT32)(((INT64)dx) * scaleX) >> 12;
  dy_m = (INT32)(((INT64)dy) * scaleY) >> 12;  

  // use magic formula (dx_m, dy_m) to get an approximation
  // See www.azillionmonkeys.com/qed/sqroot.html for ideas.
  //
  // Zillion monkeys isn't quite right. They use a min
  // where they need to use a max.
  //
  // Use prediction:  max(max(x,y), max(1/sqrt(2) * (x + y)))
  // 1/sqrt(2) is approx .7071 is approx 2896/4096
  
  predictedDist = imath_max(dx_m, dy_m);
  predictedDist = imath_max(predictedDist, 
                            ((2896 * (dx_m + dy_m)) >> 12));

  return predictedDist;
}


/// utility function to transform lat/lon data into meters.
/// The metre coordinates are relative to a third point (also in lat long)
///
/// @param scaleData (output)
///        The output scaled data in metres, relative to the origin relativePos
/// @param rawData (input)
///        The input lat/long data
/// @param relativePos (input)
///        An origin from which the scaleData is relative to. relativePos
///        is also used to generate the lat/long scaling factors. The xScale
///        factor changes depending on the latitude.
/// @return void
void World_getForwardScaleRelative(TPos *scaleData,
                                   const TPos rawData,
                                   const TPos relativePos)
{
  INT32 localXScale, localYScale;

  localXScale  = World_GetForwardScale(abs(relativePos.y) >> 16);
  localYScale  = World_GetForwardScale(0); // always the same
  scaleData->x = (localXScale * (rawData.x - relativePos.x)) >> 12;
  scaleData->y = (localYScale * (rawData.y - relativePos.y)) >> 12;
}
