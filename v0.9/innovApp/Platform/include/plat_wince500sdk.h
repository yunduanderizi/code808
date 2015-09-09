#pragma once
/*****************************************************************************
*  (c) 2007 navman new zealand limited. all rights reserved.
*
*  module name: plat_wince500sdk.h
*
*  author(s): Tom Isaacson <tom.isaacson@navman.co.nz>
*
*  description:
*    This header defines stuff for the Windows CE 5.0 SDK build.
*****************************************************************************/

#ifndef _PLAT_WINCE500SDK_H_
#define _PLAT_WINCE500SDK_H_

#ifdef PRODUCT_SDK500

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct _STORAGE_IDENTIFICATION {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwManufactureIDOffset;
    DWORD dwSerialNumOffset;
  } STORAGE_IDENTIFICATION, *PSTORAGE_IDENTIFICATION;

  // And bits for the dwFlags field:
#define MANUFACTUREID_INVALID         0x01
#define SERIALNUM_INVALID             0x02

  // Copied from winioctl.h
#define METHOD_BUFFERED               0
#define FILE_ANY_ACCESS               0
#define FILE_DEVICE_DISK              0x00000007
#define IOCTL_DISK_BASE               FILE_DEVICE_DISK
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
  ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
  )

#define IOCTL_DISK_GET_STORAGEID      CTL_CODE(IOCTL_DISK_BASE, 0x709, METHOD_BUFFERED, FILE_ANY_ACCESS)

  // Taken from winioctl.h
#define FILE_DEVICE_HAL					0x00000101
  // Taken from pkfuncs.h
#define IOCTL_HAL_GET_UUID      CTL_CODE(FILE_DEVICE_HAL, 13, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_GET_DEVICEID  CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
  BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);
  typedef struct _DEVICE_ID {
    DWORD   dwSize;
    DWORD   dwPresetIDOffset;
    DWORD   dwPresetIDBytes;
    DWORD   dwPlatformIDOffset;
    DWORD   dwPlatformIDBytes;
  } DEVICE_ID, *PDEVICE_ID;

#define IOCTL_HAL_REBOOT        CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)

  void SetCleanRebootFlag(void);

#ifdef __cplusplus
}
#endif

#endif // PRODUCT_SDK500

#endif // _PLAT_WINCE500SDK_H_
