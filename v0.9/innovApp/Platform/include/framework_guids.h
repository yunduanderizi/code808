#pragma once
#ifndef _FRAMEWORK_GUIDS_H_
#define _FRAMEWORK_GUIDS_H_

#undef DEFINE_GUID

// The final executable should define INITGUID before including the header files
#ifndef INITGUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
  /*EXTERN_C*/ extern const GUID FAR name
#else

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
  /*EXTERN_C*/ extern const GUID name \
  = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#endif // INITGUID

namespace navman {

// {16A85FB6-A535-44af-AB33-FBCD3661842A}
DEFINE_GUID(IID_IPropertyBag, 
0x16a85fb6, 0xa535, 0x44af, 0xab, 0x33, 0xfb, 0xcd, 0x36, 0x61, 0x84, 0x2a);

// {211A993E-6BFC-4875-8B85-BEB82C60AD32}
DEFINE_GUID(IID_IRectangleList, 
0x211a993e, 0x6bfc, 0x4875, 0x8b, 0x85, 0xbe, 0xb8, 0x2c, 0x60, 0xad, 0x32);

// {6E610FE1-E100-47ca-919B-840A7F6D08D1}
DEFINE_GUID(IID_IObject, 
0x6e610fe1, 0xe100, 0x47ca, 0x91, 0x9b, 0x84, 0xa, 0x7f, 0x6d, 0x8, 0xd1);

// {58DEC23C-1043-4183-A47B-C8616BA2548C}
DEFINE_GUID(IID_IAppcore,
0x58dec23c, 0x1043, 0x4183, 0xa4, 0x7b, 0xc8, 0x61, 0x6b, 0xa2, 0x54, 0x8c);

// {EE30FB04-1B50-4be0-A813-520455ED5192}
DEFINE_GUID(IID_IWorkItem, 
0xee30fb04, 0x1b50, 0x4be0, 0xa8, 0x13, 0x52, 0x4, 0x55, 0xed, 0x51, 0x92);

// {27C465EA-9F98-4e32-B659-93CC215F56DF}
DEFINE_GUID(IID_IAppNotify, 
0x27c465ea, 0x9f98, 0x4e32, 0xb6, 0x59, 0x93, 0xcc, 0x21, 0x5f, 0x56, 0xdf);

// {5C2596DB-6265-443f-A536-E1DC1CAF1A33}
DEFINE_GUID(IID_ITimeStamp, 
0x5c2596db, 0x6265, 0x443f, 0xa5, 0x36, 0xe1, 0xdc, 0x1c, 0xaf, 0x1a, 0x33);

// {E5552DC6-45B7-4c17-82B0-E395B8FD48FD}
DEFINE_GUID(IID_IXmlNode, 
0xe5552dc6, 0x45b7, 0x4c17, 0x82, 0xb0, 0xe3, 0x95, 0xb8, 0xfd, 0x48, 0xfd);

// {FCC6D4E7-092C-4c96-A8DC-2E3E3DB2B0CA}
DEFINE_GUID(IID_IXmlChildList, 
0xfcc6d4e7, 0x92c, 0x4c96, 0xa8, 0xdc, 0x2e, 0x3e, 0x3d, 0xb2, 0xb0, 0xca);

// {4B2B3242-249A-4ebc-9A03-CD0D700E0CCD}
DEFINE_GUID(IID_IXmlFile, 
0x4b2b3242, 0x249a, 0x4ebc, 0x9a, 0x3, 0xcd, 0xd, 0x70, 0xe, 0xc, 0xcd);

// {C8BE05E5-DCC3-42c3-8976-7DB1F79938A0}
DEFINE_GUID(IID_IXmlMemFile, 
0xc8be05e5, 0xdcc3, 0x42c3, 0x89, 0x76, 0x7d, 0xb1, 0xf7, 0x99, 0x38, 0xa0);

// {6D370040-00D5-4a7e-9266-AE2A2F5811CA}
DEFINE_GUID(IID_IRegfileManager, 
0x6d370040, 0xd5, 0x4a7e, 0x92, 0x66, 0xae, 0x2a, 0x2f, 0x58, 0x11, 0xca);

// {050D9211-404D-418e-9C4F-02122A6AFD85}
DEFINE_GUID(IID_IRegistryKey, 
0x50d9211, 0x404d, 0x418e, 0x9c, 0x4f, 0x2, 0x12, 0x2a, 0x6a, 0xfd, 0x85);

// {8771F638-80CA-4142-9224-B530949AF9C7}
DEFINE_GUID(IID_ISerialize, 
0x8771f638, 0x80ca, 0x4142, 0x92, 0x24, 0xb5, 0x30, 0x94, 0x9a, 0xf9, 0xc7);

// {CA02125C-7391-484c-ADAE-01D0C77A58B9}
DEFINE_GUID(IID_IPersist, 
0xca02125c, 0x7391, 0x484c, 0xad, 0xae, 0x1, 0xd0, 0xc7, 0x7a, 0x58, 0xb9);

// {3DD6D291-12D2-4adb-87FE-CCB675A94A4C}
DEFINE_GUID(IID_IMoniker, 
0x3dd6d291, 0x12d2, 0x4adb, 0x87, 0xfe, 0xcc, 0xb6, 0x75, 0xa9, 0x4a, 0x4c);

// {BA46EE3A-73EB-42f6-A5FB-19D76F8D4F08}
DEFINE_GUID(IID_IXmlNodeMoniker, 
0xba46ee3a, 0x73eb, 0x42f6, 0xa5, 0xfb, 0x19, 0xd7, 0x6f, 0x8d, 0x4f, 0x8);

// {ABA08CDF-9303-46e8-A280-DB17B442ED0B}
DEFINE_GUID(IID_ISoundManager, 
0xaba08cdf, 0x9303, 0x46e8, 0xa2, 0x80, 0xdb, 0x17, 0xb4, 0x42, 0xed, 0xb);

// {30A48781-95FA-4046-A6D6-26B7ED9FF5BF}
DEFINE_GUID(IID_ISoundManagerEventSource, 
0x30a48781, 0x95fa, 0x4046, 0xa6, 0xd6, 0x26, 0xb7, 0xed, 0x9f, 0xf5, 0xbf);

// {445C63C2-0B78-4684-AAD5-3A551669FAC7}
DEFINE_GUID(IID_ISoundManagerEventListener, 
0x445c63c2, 0xb78, 0x4684, 0xaa, 0xd5, 0x3a, 0x55, 0x16, 0x69, 0xfa, 0xc7);

// {CCA7C2A3-4D83-4f1e-8570-305C85AC2561}
DEFINE_GUID(IID_IPersistableManager, 
0xcca7c2a3, 0x4d83, 0x4f1e, 0x85, 0x70, 0x30, 0x5c, 0x85, 0xac, 0x25, 0x61);

} // namespace navman

#endif // _FRAMEWORK_GUIDS_H_
