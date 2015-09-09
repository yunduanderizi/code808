#pragma once
/**
 * \file
 * Implementation of the ISoundSource object.
 * Copyright(C) 2005 Navman New Zealand.  All rights reserved.
 */

#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#include "multimedialib.h"
#include "smart.h"

namespace navman
{
  namespace multimedia
  {
    /**
     * Interface for reading raw data.
     */
    class ISoundSource : public IObject {
    public:
      /**
       * Read source data. This function completely fills the provided
       * buffer unless there is insufficient data.
       * \param[in] buffer Pointer to destination buffer.
       * \param[in,out] size Size of destination buffer, returns the
       * size of data written to buffer.
       * \return True if successful, false otherwise.
       */
      virtual bool read(UINT8* buffer, UINT32& size) = 0;
      /**
       * Peek source data. This function completely fills the provided
       * buffer unless there is insufficient data. The peek does not
       * move the internal read position.
       * \param[in] buffer Pointer to destination buffer.
       * \param[in,out] size Size of destination buffer, returns the
       * size of data written to buffer.
       * \return True if successful, false otherwise.
       */
      virtual bool peek(UINT8* buffer, UINT32& size) = 0;
      /**
       * Peek source data at the specified position. This function
       * completely fills the provided buffer unless there is
       * insufficient data.
       * \param[in] buffer Pointer to destination buffer.
       * \param[in] position Byte offset into data.
       * \param[in,out] size Size of destination buffer, returns the
       * size of data written to buffer.
       * \return True if successful, false otherwise.
       */
      virtual bool peek(UINT8* buffer, UINT32 position, UINT32& size) = 0;
      /**
       * Move the read offset to the specified location. The next call
       * to read() will read from the specified offset.
       * \param[in,out] position Byte offset into data, returns the
       * actual offset if the given offset was past the end of the data.
       * \return True if successful, false otherwise.
       */
      virtual bool seek(UINT32& position) = 0;
      /**
       * Size of the source data.
       * \param[out] size Size of the source data.
       * \return True if successful, false otherwise.
       */
      virtual bool size(UINT32& size) = 0;
    };

    /**
     * SoundSource implementation factory.
     */
    class SoundSource {
    public:
      /**
       * Create a SoundSourceFile object - unbuffered.
       * \param[in] name Path and name of the source file.
       * \param[out] source New ISoundSource interface pointer from
       * the created object.
       * \return True if the ISoundSource interface pointer is passed
       * back, false otherwise.
       */
      static bool createFile(WCHAR const* name, SmartPtr<ISoundSource>& source);
      /**
       * Create a SoundSourceStream object - buffered.
       * \param[in] name Path and name of the source file.
       * \param[out] source New ISoundSource interface pointer from
       * the created object.
       * \return True if the ISoundSource interface pointer is passed
       * back, false otherwise.
       */
      static bool createStream(WCHAR const* name, SmartPtr<ISoundSource>& source);
      /**
       * Create a SoundSourceBuffer object.
       * \param[in] buffer Source buffer data.
       * \param[in] size Size of the source buffer data.
       * \param[out] source New ISoundSource interface pointer from
       * the created object.
       * \return True if the ISoundSource interface pointer is passed
       * back, false otherwise.
       */
      static bool createBuffer(UINT8 const* buffer, UINT32 size, SmartPtr<ISoundSource>& source);
    };
    extern "C" const GUID FAR IID_ISoundSource;
  }
}

#endif // SOUNDSOURCE_H
