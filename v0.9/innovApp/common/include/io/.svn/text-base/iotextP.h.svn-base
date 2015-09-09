#ifndef IOTEXTP_H
#define IOTEXTP_H

#include <io/iotext.h>
#include <platform.h>
#include <io/geio.h>
#include <io/iocallbacks.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup IO
/// @ingroup Utilities
/// @{

/** @typedef struct TTextStream
    Text stream for performing buffered line/character based file input e.g.
    manipulating config and ini files. */
typedef struct TTextStream {
  /** @internal
      Handle to the text data source. When text is read from a file, the
      datasource is a #OS_FILE. */
  void *datasource;
  /** @internal
      Callback functions to support reading text from a source other than a
      file. */
  TFileCallbacks cb;
  /** @internal
      File read buffer to enable efficient character based I/O. */
  char *buffer;
  /** @internal
      Size of the file read buffer in bytes. */
  UINT32 bufferSize;
  /** @internal
      Position of the next byte to be read in the buffer. */
  UINT32 bufferIndex;
  /** @internal
      Number of bytes currently in the read buffer. */
  UINT32 bytesInBuffer;
} TTextStream;

/** @internal
    Open a buffered text stream. */
extern INT32 textStream_open(TTextStream *pThis,
                             OS_FILE datasource,
                             char *buffer,
                             UINT32 bufferSize,
                             INT32 direction);

/** @internal
    Establish callbacks for manipulating text stream. */
extern INT32 textStream_openCallbacks(TTextStream *pThis,
                                      void *datasource,
                                      char *buffer,
                                      UINT32 bufferSize,
                                      INT32 direction,
                                      const TFileCallbacks *cb);

/** @internal
*/
extern INT32 textStream_clear(TTextStream *pThis);

/** @internal
*/
extern INT32 textStream_readChar(TTextStream *pThis);

/** @internal
*/
extern INT32 textStream_readLine(TTextStream *pThis,
                                 char *line,
                                 INT32 lineBufferSize);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* IOTEXTP_H */
