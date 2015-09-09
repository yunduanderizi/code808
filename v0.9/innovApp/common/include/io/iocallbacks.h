#ifndef IOCALLBACKS_H
#define IOCALLBACKS_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif
  
/// @addtogroup IO
/// @ingroup Utilities
/// @{

/** File callback functions. These allow the caller to implement custom file
    manipulation routines. For example, if there is a lot of free memory, a
    data file may be loaded and read directly from RAM, instead of from disk.
    This may help to improve performance in some situations on some platforms.

    @note The prototypes for the callback functions are the same as for the
    platform functions os_fread, os_fseek, os_fclose, and os_ftell. The
    behaviour of these functions is essentially the same as for the
    stdio functions fread, fseek, fclose, and ftell. */
typedef struct {
  /** Read function. This should match the behaviour of #os_fread. */
  UINT32 (*read_func)(void *buffer, UINT32 nbytes, void *stream);
  /** Seek function. This should match the behaviour of #os_fseek. */
  UINT32 (*seek_func)(void *stream, INT32 offset, UINT32 origin);
  /** Close function. This should match the behaviour of #os_fclose. */
  UINT32 (*close_func)(void *stream);
  /** Tell function. This should match the behaviour of #os_ftell. */
  INT32 (*tell_func)(void *stream);
} TFileCallbacks;

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif /* IOCALLBACKS_H */
