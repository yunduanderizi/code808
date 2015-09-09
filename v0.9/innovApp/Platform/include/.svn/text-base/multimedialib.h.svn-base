#pragma once
#ifndef MULTIMEDIALIB_H
#define MULTIMEDIALIB_H

#include <product_features.h>
#include "frameworklib.h"

/*
 * Include the following header when running the test configuration.
 */
#if defined(_TEST)
  #include "../test/include/multimediatest.h"
#endif

// ogg vorbis sound file support flag for mapcore
#define PLATFORM_VORBIS_SUPPORT 1

namespace navman
{
  namespace multimedia
  {
    /*
     *  Nuance TTS defines
     */
    #if defined(_WIN32_WCE)
      #define SSFT_UNICODE
    #elif defined(WIN32)
      #undef SSFT_UNICODE
    #endif
  }
}

#endif // MULTIMEDIALIB_H
