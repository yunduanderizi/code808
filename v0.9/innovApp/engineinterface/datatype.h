#ifndef DATATYPE_H
#define DATATYPE_H
#include"platform_types.h"

///////////////////////////////////////////////////
////All the followings for colour pattern in map interface
typedef struct TMapColourEntry
{
    const wchar_t *legend;
    TDisplayColours colourIndex;
} TMapColourEntry;

typedef struct TElevationMapColourEntry
{
    bool enabled;
    INT32 height;
    UINT8 red;
    UINT8 green;
    UINT8 blue;
} TElevationMapColourEntry;

const unsigned int numberOfGradients = 8;

typedef struct TElevationMapColourDescriptor
{
    unsigned int size;
    TElevationMapColourEntry entry[ numberOfGradients ];
} TElevationMapColourDescriptor;

typedef struct TPolyMapColourEntry
{
    const wchar_t *polyName;
    TPolClass polyClass;
} TPolyMapColourEntry;

static TMapColourEntry g_roadColourTable[] =
{
  // 16 miscellaneous colours
  { L"misc_route",                     DISPLAY_COLOUR_MISC_ROUTE_INDEX                      },
  { L"misc_poi",                       DISPLAY_COLOUR_MISC_POI_INDEX                        },
  { L"misc_poi_outline",               DISPLAY_COLOUR_MISC_POI_OUTLINE_INDEX                },
  { L"misc_major_label_border",        DISPLAY_COLOUR_MISC_MAJOR_LABEL_BORDER_INDEX         },
  { L"misc_major_label_fill",          DISPLAY_COLOUR_MISC_MAJOR_LABEL_FILL_INDEX           },
  { L"misc_major_label_text",          DISPLAY_COLOUR_MISC_MAJOR_LABEL_TEXT_INDEX           },
  { L"misc_turn_future",               DISPLAY_COLOUR_MISC_TURN_FUTURE_INDEX                },
  { L"misc_turn_hilight (obsolete)",   DISPLAY_COLOUR_MISC_TURN_HILIGHT_INDEX               },
  { L"misc_settlement_town",           DISPLAY_COLOUR_MISC_SETTLEMENT_TOWN_INDEX            },
  { L"misc_settlement_city",           DISPLAY_COLOUR_MISC_SETTLEMENT_CITY_INDEX            },
  { L"misc_settlement_major_city",     DISPLAY_COLOUR_MISC_SETTLEMENT_MAJOR_CITY_INDEX      },
  { L"misc_settlement_national_city",  DISPLAY_COLOUR_MISC_SETTLEMENT_NATIONAL_CITY_INDEX   },
  { L"misc_settlement_outline",        DISPLAY_COLOUR_MISC_SETTLEMENT_OUTLINE_INDEX         },
  { L"misc_turn_past",                 DISPLAY_COLOUR_MISC_TURN_PAST_INDEX                  },
  { L"misc_road_select",               DISPLAY_COLOUR_MISC_ROAD_SELECT_INDEX                },
  { L"misc_route_direction",           DISPLAY_COLOUR_MISC_ROUTE_DIRECTION_INDEX            },
  // 16 road colours
  { L"road_motorway",                  DISPLAY_COLOUR_ROAD_MOTORWAY_INDEX                   },
  { L"road_nationalhighway",           DISPLAY_COLOUR_ROAD_NATIONALHIGHWAY_INDEX            },
  { L"road_regionalhighway",           DISPLAY_COLOUR_ROAD_REGIONALHIGHWAY_INDEX            },
  { L"road_importantmajor",            DISPLAY_COLOUR_ROAD_IMPORTANTMAJOR_INDEX             },
  { L"road_major",                     DISPLAY_COLOUR_ROAD_MAJOR_INDEX                      },
  { L"road_minor",                     DISPLAY_COLOUR_ROAD_MINOR_INDEX                      },
  { L"road_lane",                      DISPLAY_COLOUR_ROAD_LANE_INDEX                       },
  { L"road_pedestrian",                DISPLAY_COLOUR_ROAD_PEDESTRIAN_INDEX                 },
  { L"road_border_motorway",           DISPLAY_COLOUR_ROAD_BORDER_MOTORWAY_INDEX            },
  { L"road_border_nationalhighway",    DISPLAY_COLOUR_ROAD_BORDER_NATIONALHIGHWAY_INDEX     },
  { L"road_border_regionalhighway",    DISPLAY_COLOUR_ROAD_BORDER_REGIONALHIGHWAY_INDEX     },
  { L"road_border_importantmajor",     DISPLAY_COLOUR_ROAD_BORDER_IMPORTANTMAJOR_INDEX      },
  { L"road_border_major",              DISPLAY_COLOUR_ROAD_BORDER_MAJOR_INDEX               },
  { L"road_border_minor",              DISPLAY_COLOUR_ROAD_BORDER_MINOR_INDEX               },
  { L"road_border_lane",               DISPLAY_COLOUR_ROAD_BORDER_LANE_INDEX                },
  { L"road_border_pedestrian",         DISPLAY_COLOUR_ROAD_BORDER_PEDESTRIAN_INDEX          },
  // 16 arc classes
  { L"arc_land",                       DISPLAY_COLOUR_ARC_LAND_INDEX                        },
  { L"arc_water",                      DISPLAY_COLOUR_ARC_WATER_INDEX                       },
  { L"arc_rail",                       DISPLAY_COLOUR_ARC_RAIL_INDEX                        },
  { L"arc_pedestrian",                 DISPLAY_COLOUR_ARC_PEDESTRIAN_INDEX                  },
  { L"arc_boundary",                   DISPLAY_COLOUR_ARC_BOUNDARY_INDEX                    },
  { L"arc_ferry",                      DISPLAY_COLOUR_ARC_FERRY_INDEX                       },
  { L"arc_wharf",                      DISPLAY_COLOUR_ARC_WHARF_INDEX                       },
  { L"arc_proposed_road",              DISPLAY_COLOUR_ARC_PROPOSED_ROAD_INDEX               },
  { L"arc_class8",                     DISPLAY_COLOUR_ARC_CLASS8_INDEX                      },
  { L"arc_class9",                     DISPLAY_COLOUR_ARC_CLASS9_INDEX                      },
  { L"arc_class10",                    DISPLAY_COLOUR_ARC_CLASS10_INDEX                     },
  { L"arc_class11",                    DISPLAY_COLOUR_ARC_CLASS11_INDEX                     },
  { L"arc_class12",                    DISPLAY_COLOUR_ARC_CLASS12_INDEX                     },
  { L"arc_class13",                    DISPLAY_COLOUR_ARC_CLASS13_INDEX                     },
  { L"arc_class14",                    DISPLAY_COLOUR_ARC_CLASS14_INDEX                     },
  { L"arc_class15",                    DISPLAY_COLOUR_ARC_CLASS15_INDEX                     },
  // 16 more miscellaneous classes
  { L"misc_road_toll",                 DISPLAY_COLOUR_MISC_ROAD_TOLL_INDEX                  },
  { L"misc_road_freeway",              DISPLAY_COLOUR_MISC_ROAD_FREEWAY_INDEX               },
  { L"misc_road_unsealed",             DISPLAY_COLOUR_MISC_ROAD_UNSEALED_INDEX              },
  { L"misc_road_non_driveable",        DISPLAY_COLOUR_MISC_ROAD_NON_DRIVEABLE_INDEX         },
  { L"misc_road_ferry",                DISPLAY_COLOUR_MISC_ROAD_FERRY_INDEX                 },
  { L"misc_class21",                   DISPLAY_COLOUR_MISC_CLASS21_INDEX                    },
  { L"misc_class22",                   DISPLAY_COLOUR_MISC_CLASS22_INDEX                    },
  { L"misc_class23",                   DISPLAY_COLOUR_MISC_CLASS23_INDEX                    },
  { L"misc_road_border_toll",          DISPLAY_COLOUR_MISC_ROAD_BORDER_TOLL_INDEX           },
  { L"misc_road_border_freeway",       DISPLAY_COLOUR_MISC_ROAD_BORDER_FREEWAY_INDEX        },
  { L"misc_road_border_unsealed",      DISPLAY_COLOUR_MISC_ROAD_BORDER_UNSEALED_INDEX       },
  { L"misc_road_border_non_driveable", DISPLAY_COLOUR_MISC_ROAD_BORDER_NON_DRIVEABLE_INDEX  },
  { L"misc_road_border_ferry",         DISPLAY_COLOUR_MISC_ROAD_BORDER_FERRY_INDEX          },
  { L"misc_class29",                   DISPLAY_COLOUR_MISC_CLASS29_INDEX                    },
  { L"misc_class30",                   DISPLAY_COLOUR_MISC_CLASS30_INDEX                    },
  { L"misc_class31",                   DISPLAY_COLOUR_MISC_CLASS31_INDEX                    },
  // Current turn
  { L"misc_turn_current",              DISPLAY_COLOUR_MISC_TURN_CURRENT_INDEX               },
  // Traffic colours
  { L"traffic_slow",                   DISPLAY_COLOUR_TRAFFIC_SLOW_INDEX                    },
  { L"traffic_heavy",                  DISPLAY_COLOUR_TRAFFIC_HEAVY_INDEX                   },
  { L"traffic_blocked",                DISPLAY_COLOUR_TRAFFIC_BLOCKED_INDEX                 },
  { L"traffic_chevrons",               DISPLAY_COLOUR_TRAFFIC_CHEVRONS_INDEX                },
  // Distant 3D roads
  { L"misc_3D_distant_roads",          DISPLAY_COLOUR_MISC_DISTANT_ROADS_IN_3D_INDEX        },
  // Non-highway road label colours
  { L"label_text",                     DISPLAY_COLOUR_LABEL_TEXT_INDEX                      },
  { L"label_fill",                     DISPLAY_COLOUR_LABEL_FILL_INDEX                      },
  { L"label_frame",                    DISPLAY_COLOUR_LABEL_FRAME_INDEX                     },
  { L"label_frameshadow",              DISPLAY_COLOUR_LABEL_FRAME_SHADOW_INDEX              },
  { L"label_anchor",                   DISPLAY_COLOUR_LABEL_ANCHOR_INDEX                    },
  { L"label_anchor_border",            DISPLAY_COLOUR_LABEL_ANCHOR_BORDER_INDEX             },
  { L"label_settlement_text",          DISPLAY_COLOUR_LABEL_SETTLEMENT_TEXT_INDEX           },
  { L"label_settlement_fill",          DISPLAY_COLOUR_LABEL_SETTLEMENT_FILL_INDEX           },
  // Sky and fog colours
  { L"sky_gradient_upper",             DISPLAY_COLOUR_SKY_GRADIENT_UPPER_INDEX              },
  { L"sky_gradient_lower",             DISPLAY_COLOUR_SKY_GRADIENT_LOWER_INDEX              },
  // 3D building colours
  { L"textured_3d_building",           DISPLAY_COLOUR_TEXTURED_3D_BUILDING_INDEX            },
  { L"solid_3d_building",              DISPLAY_COLOUR_SOLID_3D_BUILDING_INDEX               },
  { L"transparent_3d_building",        DISPLAY_COLOUR_TRANSPARENT_3D_BUILDING_INDEX         },
  { L"wireframe_3d_building",          DISPLAY_COLOUR_WIREFRAME_3D_BUILDING_INDEX           },
  { L"end_of_world",                   DISPLAY_COLOUR_END_OF_WORLD_INDEX                    },
  // debug colours
  { L"stats_fps_text",                 DISPLAY_COLOUR_FPS_TEXT_INDEX                        },
  { L"stats_fps_fill",                 DISPLAY_COLOUR_FPS_FILL_INDEX                        },
  { L"stats_gps_text",                 DISPLAY_COLOUR_GPS_STATS_TEXT_INDEX                  },
  { L"stats_gps_fill",                 DISPLAY_COLOUR_GPS_STATS_FILL_INDEX                  },
  { L"gps_arrow_fill",                 DISPLAY_COLOUR_GPS_ARROW_FILL_INDEX                  },
  { L"gps_arrow_border",               DISPLAY_COLOUR_GPS_ARROW_BORDER_INDEX                },
  // more misc colours
  { L"misc_turn_border",               DISPLAY_COLOUR_MISC_TURN_BORDER_INDEX                },
  { L"one_way_arrow_fill",             DISPLAY_COLOUR_ONE_WAY_ARROW_FILL_INDEX              },
  { L"one_way_arrow_border",           DISPLAY_COLOUR_ONE_WAY_ARROW_BORDER_INDEX            },
  { L"lat_lon_grid",                   DISPLAY_COLOUR_LAT_LON_GRID_INDEX                    },
  { L"highway_shield_text",            DISPLAY_COLOUR_HIGHWAY_SHIELD_TEXT_INDEX             },
  { L"highway_shield_border",          DISPLAY_COLOUR_HIGHWAY_SHIELD_BORDER_INDEX           },
  { L"highway_shield_fill",            DISPLAY_COLOUR_HIGHWAY_SHIELD_FILL_INDEX             },
  // colours for city map arcs
  { L"city_building_decorative",       DISPLAY_COLOUR_CITY_BUILDING_DECORATIVE              },
  { L"city_building_ridge",            DISPLAY_COLOUR_CITY_BUILDING_RIDGE                   },
  { L"city_tblock_border",             DISPLAY_COLOUR_CITY_TBLOCK_BORDER                    },
  { L"city_tblock_sub_div",            DISPLAY_COLOUR_CITY_TBLOCK_SUB_DIV                   },
  { L"city_rail_default",              DISPLAY_COLOUR_CITY_RAIL_DEFAULT                     },
  { L"city_rail_default_underground",  DISPLAY_COLOUR_CITY_RAIL_DEFAULT_UNDERGROUND         },
  { L"city_rail_tram",                 DISPLAY_COLOUR_CITY_RAIL_TRAM                        },
  { L"city_rail_tram_underground",     DISPLAY_COLOUR_CITY_RAIL_TRAM_UNDERGROUND            },
  { L"city_rail_subway",               DISPLAY_COLOUR_CITY_RAIL_SUBWAY                      },
  { L"city_rail_subway_underground",   DISPLAY_COLOUR_CITY_RAIL_SUBWAY_UNDERGROUND          },
};

static TPolyMapColourEntry g_polyColourTable[ ] =
{
  { L"poly_default",                    POL_CLASS_DEFAULT                         },
  { L"poly_park",                       POL_CLASS_PARK                            },
  { L"poly_water",                      POL_CLASS_WATER                           },
  { L"poly_building",                   POL_CLASS_BUILDING                        },
  { L"poly_academic",                   POL_CLASS_ACADEMIC                        },
  { L"poly_runway",                     POL_CLASS_RUNWAY                          },
  { L"poly_racecourse",                 POL_CLASS_RACECOURSE                      },
  { L"poly_private_property",           POL_CLASS_PRIVATE                         },
  { L"poly_hospital",                   POL_CLASS_HOSPITAL                        },
  { L"poly_pedestrian",                 POL_CLASS_PEDESTRIAN                      },
  { L"poly_wharf",                      POL_CLASS_WHARF                           },
  { L"poly_sand",                       POL_CLASS_SAND                            },
  { L"poly_moor",                       POL_CLASS_MOOR                            },
  { L"poly_woodland",                   POL_CLASS_WOODLAND                        },
  { L"poly_coverage",                   POL_CLASS_COVERAGE                        },
  { L"poly_airport",                    POL_CLASS_AIRPORT_GROUND                  },
  { L"poly_congestion",                 POL_CLASS_CONGESTION                      },
  { L"poly_golfcourse",                 POL_CLASS_GOLF_COURSE                     },
  { L"poly_builtuparea",                POL_CLASS_BUILT_UP_AREA                   },

  { L"city_building_default",           POL_CLASS_CITY_BUILDING_DEFAULT           },
  { L"city_building_commercial",        POL_CLASS_CITY_BUILDING_COMMERCIAL        },
  { L"city_building_cultural",          POL_CLASS_CITY_BUILDING_CULTURAL          },
  { L"city_building_factory",           POL_CLASS_CITY_BUILDING_FACTORY           },
  { L"city_building_public_office",     POL_CLASS_CITY_BUILDING_PUBLIC_OFFICE     },
  { L"city_building_hospital",          POL_CLASS_CITY_BUILDING_HOSPITAL          },
  { L"city_building_hotel",             POL_CLASS_CITY_BUILDING_HOTEL             },
  { L"city_building_multi_carpark",     POL_CLASS_CITY_BUILDING_MULTI_CARPARK     },
  { L"city_building_other",             POL_CLASS_CITY_BUILDING_OTHER             },
  { L"city_building_ped_deck",          POL_CLASS_CITY_BUILDING_PED_DECK          },
  { L"city_building_worship",           POL_CLASS_CITY_BUILDING_WORSHIP           },
  { L"city_building_railway_platform",  POL_CLASS_CITY_BUILDING_RAILWAY_PLATFORM  },
  { L"city_building_railway",           POL_CLASS_CITY_BUILDING_RAILWAY           },
  { L"city_building_railway_ug",        POL_CLASS_CITY_BUILDING_RAILWAY_UG        },
  { L"city_building_school",            POL_CLASS_CITY_BUILDING_SCHOOL            },
  { L"city_building_subway_platform",   POL_CLASS_CITY_BUILDING_SUBWAY_PLATFORM   },
  { L"city_building_subway",            POL_CLASS_CITY_BUILDING_SUBWAY            },
  { L"city_building_subway_ug",         POL_CLASS_CITY_BUILDING_SUBWAY_UG         },

  { L"city_tblock_default",             POL_CLASS_CITY_TBLOCK_DEFAULT             },
  { L"city_tblock_shore",               POL_CLASS_CITY_TBLOCK_SHORE               },
  { L"city_tblock_divider",             POL_CLASS_CITY_TBLOCK_DIVIDER             },
  { L"city_tblock_factory",             POL_CLASS_CITY_TBLOCK_FACTORY             },
  { L"city_tblock_forest",              POL_CLASS_CITY_TBLOCK_FOREST              },
  { L"city_tblock_garden_path",         POL_CLASS_CITY_TBLOCK_GARDEN_PATH         },
  { L"city_tblock_grass",               POL_CLASS_CITY_TBLOCK_GRASS               },
  { L"city_tblock_green_urban",         POL_CLASS_CITY_TBLOCK_GREEN_URBAN         },
  { L"city_tblock_hospital",            POL_CLASS_CITY_TBLOCK_HOSPITAL            },
  { L"city_tblock_marsh",               POL_CLASS_CITY_TBLOCK_MARSH               },
  { L"city_tblock_paved",               POL_CLASS_CITY_TBLOCK_PAVED               },
  { L"city_tblock_railway",             POL_CLASS_CITY_TBLOCK_RAILWAY             },
  { L"city_tblock_sand",                POL_CLASS_CITY_TBLOCK_SAND                },
  { L"city_tblock_school",              POL_CLASS_CITY_TBLOCK_SCHOOL              },
  { L"city_tblock_pool",                POL_CLASS_CITY_TBLOCK_POOL                },
  { L"city_tblock_walkway",             POL_CLASS_CITY_TBLOCK_WALKWAY             },
  { L"city_tblock_water",               POL_CLASS_CITY_TBLOCK_WATER               },
  { L"city_roads",                      POL_CLASS_CITY_ROADS                      },
  { L"elevation",                       POL_CLASS_ELEVATION                       },
};

const static wchar_t * STR_GRADIENT[numberOfGradients] = {
  L"gradient_1", L"gradient_2", L"gradient_3", L"gradient_4",
  L"gradient_5", L"gradient_6", L"gradient_7", L"gradient_8"
};

enum ColourType
{                                                     //for color handling
    POLY_CITY, ROAD_ARC_MISC, GRADIENT_ENABLE, GRADIENT_DISABLE, NOT_DEFINED
};
struct Colour
{
    int red, green, blue, alpha;
};
struct ColourUnit
{
    ColourType type;
    Colour colour;
    size_t typeIndex;                                                  //index the color in g_navDisplay
    ColourUnit()
    {
        type = NOT_DEFINED;
        colour.alpha = 0;
    }
};
enum ColourTagType
{                                                  //identify colour tags
    UNKNOWN, DAY, NIGHT
};

struct StrCmpHash
{                                                   //hash function for string
    const static size_t bucket_size = 4;                               //convention in hash_compare
    const static size_t min_buckets = 8;                               //convention in hash_compare
    size_t operator( )( const wchar_t* key ) const
    {                    //a simple hash function
        size_t hash = 0;
        for(size_t value; value = *key++; hash = (hash << 5) - hash + value);
        return hash;
    }
    bool operator( )(const wchar_t* s1, const wchar_t* s2) const
    {
        return wcscmp(s1, s2) != 0;
    }
};
typedef pair<const wchar_t*, ColourUnit> ColourPair;


///////////////////////////////////////////////////
////All the followings for search interface
#define NUM_SEARCH_RESULT   50
#define SEARCH_INTERVAL     1000
#define TLE_LEN             72
#define STR_LEN             1024
#define NUM_PARAMS          11

#define VERIFY(f)          assert(f)
const static UINT32 NEAR_POI_SCH_SIZE = 2000;

typedef unsigned short WCHAR;
typedef unsigned long       DWORD;

enum SEARCH_LEVEL
{
    SCH_PROVINCE_LEVEL = 0,
    SCH_CITY_LEVEL,
    SCH_POI_LEVEL,
    SCH_ROAD_LEVEL,
    SCH_CROSS_LOAD_LEVEL,
    SCH_POI_TYPE_LEVEL,
    SCH_NO_SEARCH_LEVEL
};

enum START_SEARCH_TYPE
{
    SCH_DEFAUT_SEARCH_TYPE = 0,
    SCH_POI_CATEGORY_SEARCH_TYPE,
    SCH_POI_AROUND_SEARCH_TYPE
};

enum TEXT_TYPE
{
    SCH_TEXT_TYPE_NAME,
    SCH_TEXT_TYPE_LOCATION_NAME
};

enum AROUND_SCH_POI_TYPE
{
    SCH_AROUND_POI_TYPE_DEFAULT = 0,
    SCH_AROUND_POI_TYPE_GASSTATION = 285,
    SCH_AROUND_POI_TYPE_PARKING = 303,
    SCH_AROUND_POI_TYPE_RESTAURANT = 311,
    SCH_AROUND_POI_TYPE_HOTEL = 300,
    SCH_AROUND_POI_TYPE_HOSTPITAL = 265,
    SCH_AROUND_POI_TYPE_SUPERMAKET = 320,
    SCH_AROUND_POI_TYPE_BANK = 277,
    SCH_AROUND_POI_TYPE_CARSERVICE = 256,
    SCH_AROUND_POI_TYPE_SERVICE = 53
};

enum CATEGORY_POI_TYPE
{
    SCH_CATEGORY_POI_TYPE_DEFAULT = 0,
    SCH_CATEGORY_POI_TYPE_GASSTATION = 285,
    SCH_CATEGORY_POI_TYPE_PARKING = 303,
    SCH_CATEGORY_POI_TYPE_RESTAURANT = 311,
    SCH_CATEGORY_POI_TYPE_HOTEL = 300,
    SCH_CATEGORY_POI_TYPE_HOSTPITAL = 265,
    SCH_CATEGORY_POI_TYPE_SUPERMAKET = 320,
    SCH_CATEGORY_POI_TYPE_BANK = 277,
    SCH_CATEGORY_POI_TYPE_CARSERVICE = 256,
    SCH_CATEGORY_POI_TYPE_SERVICE = 53
};

typedef struct
{
    BOOL8 country;
    BOOL8 province;
    BOOL8 city;
    BOOL8 subCity;
    BOOL8 postcode;
    BOOL8 dPostcode;
    BOOL8 road;
    BOOL8 houseNo;
    BOOL8 intersection;
    BOOL8 poi;
    BOOL8 poiType;
    BOOL8 lat_lon;
}structRealLevel;

typedef struct
{
    UINT32 uDistance;
    UTF16 tel[TLE_LEN];
    UTF16 address[STR_LEN];
    UTF16 name[STR_LEN];
}structDetailInfo;

typedef struct
{
    int nProvinceIdx;
    int nCityIdx;
}structAdminIdx;

///////////////////////////////////////////////////
////All the followings for engine interface
typedef enum GUIDE_VALUE
{
    GUIDE_DEFAULT = 0,
    GUIDE_START_OF_SIM,
    GUIDE_END_OF_SIM,
    GUIDE_START_OF_REAL,
    GUIDE_END_OF_REAL
}GUIDE_VALUE;

#define GPSLOG_OFFROUTE_CAUSE_REROUTE 3
#define GUIDE_LIST_DEFAULT -2
#define GUIDE_LIST_ERROR -1
#if defined(ARM_LINUX)
#define GRAMMER_FILE_PATH "//lib//chinese_cn.so"
#else
#define GRAMMER_FILE_PATH "//home//administrator//maps//china//build002//chinese_cn.so"
#endif

//actually lookup_file_path now is defined in lib
#if defined(ARM_LINUX)
#define LOOKUP_FILE_PATH "//application//work//lookup.bin"
#else
#define LOOKUP_FILE_PATH "//home//administrator//maps//china//build002//lookup.bin"
#endif
///////////////////////////////////////////////////
////All the followings for guide interface

typedef enum TURN_DEGREE
{
    TURN_STRAIGHT = 0,
    TURN_SLIGHT,
    TURN_MEDIUM,
    TURN_SHARP
}TURN_DEGREE;

#define JUNCTIONVIEW_PIC_SIEZ 100*1024

#endif // DATATYPE_H
