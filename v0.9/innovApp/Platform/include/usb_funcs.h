#ifndef _USB_FUNCS_H_
#define _USB_FUNCS_H_

#if defined(_WIN32_WCE) && defined(TARGET) && !defined(PRODUCT_SDK500)

typedef enum
{
  USB_UNKNOWN       = 0,
  USB_SERIAL        = 1,
  USB_MASS_STORAGE  = 2,

  USB_TYPE_MAX
} TUSBType;

BOOL usb_isAvailable(TUSBType p_eUSBType);

BOOL usb_setCurrent(TUSBType p_eUSBType);
TUSBType usb_getCurrent(void);

BOOL usb_sendReadyForStart(void);

BOOL usb_turnOn(void);
BOOL usb_turnOff(void);

#endif

#endif // _USB_FUNCS_H_
