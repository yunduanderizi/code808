/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_geosearchP_h
#define INCLUDE_GUARD_geosearchP_h

#include <geosearch/geosearch.h>

#if defined(__cplusplus)
extern "C" {
#endif

//--------------------------------------------------------------------------------
/// @internal
/// @brief This type is used to represent a single place column.
typedef UINT8 TGeosearchPlaceColumn;

/// @internal
enum {
  /// @brief Used to specify the BUA place column.
  GEOSEARCH_PLACE_COLUMN_BUA     = 0,

  /// @brief Used to specify the A9 place column.
  GEOSEARCH_PLACE_COLUMN_A9      = 1,

  /// @brief Used to specify the A8 place column.
  GEOSEARCH_PLACE_COLUMN_A8      = 2,

  /// @brief Used to specify the A7 place column.
  GEOSEARCH_PLACE_COLUMN_A7      = 3,

  /// @brief Used to specify the A2 place column.
  GEOSEARCH_PLACE_COLUMN_A2      = 4,

  /// @brief Used to specify the A1 place column.
  GEOSEARCH_PLACE_COLUMN_A1      = 5,

  /// @brief Used to specify the State place column.
  GEOSEARCH_PLACE_COLUMN_STATE   = 5,

  /// @brief Used to specify the A0 place column.
  GEOSEARCH_PLACE_COLUMN_A0      = 6,

  /// @brief Used to specify the Country place column.
  GEOSEARCH_PLACE_COLUMN_COUNTRY = 6,

  /// @brief Used to specify the area postal code place column.
  GEOSEARCH_PLACE_COLUMN_POCODE  = 7,

  /// @brief Used to specify the priority-1 place column.
  GEOSEARCH_PLACE_COLUMN_P1      = 0,

  /// @brief Used to specify the priority-2 place column.
  GEOSEARCH_PLACE_COLUMN_P2      = 1,

  /// @brief Used to specify the priority-3 place column.
  GEOSEARCH_PLACE_COLUMN_P3      = 2,

  /// @brief Used to specify the priority-4 place column.
  GEOSEARCH_PLACE_COLUMN_P4      = 3,

  /// @brief Used to specify the priority-5 place column.
  GEOSEARCH_PLACE_COLUMN_P5      = 4,

  /// @brief Used to specify an invalid place column.
  GEOSEARCH_PLACE_COLUMN_INVALID = 0xFF
};

//--------------------------------------------------------------------------------
/// @internal
/// @brief This type is used to represent a set of place columns.
typedef UINT8 TGeosearchPlaceColumnMask;

/// @internal
enum {
  /// @brief Used to specify the BUA place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_BUA     = 0x01,

  /// @brief Used to specify the A9 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_A9      = 0x02,

  /// @brief Used to specify the A8 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_A8      = 0x04,

  /// @brief Used to specify the A7 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_A7      = 0x08,

  /// @brief Used to specify the A2 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_A2      = 0x10,

  /// @brief Used to specify the A1 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_A1      = 0x20,

  /// @brief Used to specify the State place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_STATE   = 0x20,

  /// @brief Used to specify the A0 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_A0      = 0x40,

  /// @brief Used to specify the Country place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_COUNTRY = 0x40,

  /// @brief Used to specify the area postal code place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_POCODE  = 0x80,

  /// @brief Used to specify a mask of all searchable place columns
  ///        (A2, A7, A8, A9, BUA).
  GEOSEARCH_PLACE_COLUMN_MASK_PLACE   = 0x1F,

  /// @brief Used to specify a mask of all searchable area columns
  ///        (A1, A2, A7, A8, A9, BUA).
  GEOSEARCH_PLACE_COLUMN_MASK_AREA    = 0x3F,

  /// @brief Used to specify the priority-1 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_P1      = 0x01,

  /// @brief Used to specify the priority-2 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_P2      = 0x02,

  /// @brief Used to specify the priority-3 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_P3      = 0x04,

  /// @brief Used to specify the priority-4 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_P4      = 0x08,

  /// @brief Used to specify the priority-5 place column mask.
  GEOSEARCH_PLACE_COLUMN_MASK_P5      = 0x10
};

//--------------------------------------------------------------------------------
BOOL8 geosearch_init( void );

//--------------------------------------------------------------------------------
#if defined(__cplusplus)
} //extern "C"
#endif

#endif /*INCLUDE_GUARD_geosearchP_h*/
