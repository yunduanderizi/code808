#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#ifdef __cplusplus
extern "C" {
#endif

  
  /// @addtogroup Display
/// @{

///@addtogroup UnUsed
/// @ingroup Display
/// @{

///@addtogroup ComponentModel
/// @ingroup UnUsed
/// @{

/// @def MDECLARE_INTERFACE
/// MDECLARE_INTERFACE declarations define an interface in a language-independent way.
/// Depending on whether the header file is included from a C or C++ source file,
/// these macros will expand to either a structure with function pointers in C
/// or a class with pure abstract methods in C++.
/// This makes it possible to continue developing in C in MapCore while providing
/// a friendly C++-compatible API.
/// This also makes it possible to develop through interfaces and have multiple
/// components implement the same interfaces.
/// The virtual function table is binary compatible with COM when compiled
/// with Microsoft compilers and also supports MSVC's RunTime Type Information (RTTI)
/// although I have never tested that.
///
/// Macros like these ones were often found in VC++ 4.0 (1995)
/// default Platform SDK header files and are still shipped with ATL (see objbase.h).
/// They are also used for Wine development and kind of inspired Mozilla for the XPCOM
/// object model (a pale copy of COM).
///
/// MCINTERFACE can be defined to force explicit C vtable binding through
/// the lpVtbl pointer (eg. the implicit __vfptr member in VC++) even though
/// you have __cplusplus defined.
///
/// @code
/// #undef MINTERFACE
/// #define MINTERFACE IMyInterface
/// MDECLARE_INTERFACE(IMyInterface)
/// {
///   MBEGIN_INTERFACE
///   MSTDMETHOD(MyMethod1)(MTHIS) MPURE;
///   MSTDMETHOD_(BOOL8,MyMethod2)(MTHIS_ UINT32 foobar) MPURE;
///   MBEGIN_INTERFACE
/// }
/// @endcode
/// expands to:
/// @code
/// struct IMyInterface
/// {
///   void (*MyMethod1)(IMyInterface *iface);
///   BOOL8 (*MyMethod2)(IMyInterface *iface, UINT32 foobar);
/// }
/// @endcode
/// in C but expands to:
/// @code
/// class IMyInterface
/// {
///   void MyMethod1();
///   BOOL8 MyMethod2(UINT32 foobar);
/// }
/// @endcode
/// in C++ (provided MCINTERFACE is not defined).
/// Note that class and struct are perfect synonyms in C++.
///
/// See IRoadTheme for an example implementation in C of the interface.

/// @}
/// @}
/// @}
// #define ICOM_MSVTABLE_COMPAT 1
// #define ICOM_USE_COM_INTERFACE_ATTRIBUTE 1

#if defined(__cplusplus) && !defined(MCINTERFACE) && 0

#  define MRETURNTYPE void
#  define MSTDMETHODCALLTYPE

#  define MSTDMETHOD(method)        virtual MRETURNTYPE MSTDMETHODCALLTYPE method
#  define MSTDMETHOD_(type,method)  virtual type MSTDMETHODCALLTYPE method
#  define MSTDMETHODV(method)       virtual MRETURNTYPE MSTDMETHODVCALLTYPE method
#  define MSTDMETHODV_(type,method) virtual type MSTDMETHODVCALLTYPE method

#  define MPURE   = 0
#  define MTHIS_
#  define MTHIS   void

#  ifdef interface
#    undef interface
#  endif
#  define interface struct

#  ifdef ICOM_USE_COM_INTERFACE_ATTRIBUTE
#    define MDECLARE_INTERFACE(iface)        interface __attribute__((com_interface)) iface
#  else
#    define MDECLARE_INTERFACE(iface)        interface iface
#  endif
#  define MDECLARE_INTERFACE_(iface,ibase) interface iface : public ibase

#  define MBEGIN_INTERFACE
#  define MEND_INTERFACE

#  define ICOM_MSVTABLE_COMPAT_DummyRTTIVALUE

#else  /* __cplusplus && !CINTERFACE */

#  define MRETURNTYPE void
#  define MSTDMETHODCALLTYPE

#  define COBJMACROS

#  define MSTDMETHOD(method)        MRETURNTYPE (MSTDMETHODCALLTYPE *method)
#  define MSTDMETHOD_(type,method)  type (MSTDMETHODCALLTYPE *method)
#  define MSTDMETHODV(method)       MRETURNTYPE (MSTDMETHODVCALLTYPE *method)
#  define MSTDMETHODV_(type,method) type (MSTDMETHODVCALLTYPE *method)

#  define MPURE
#  define MTHIS_ struct MINTERFACE *This,
#  define MTHIS  struct MINTERFACE *This

#  ifdef interface
#    undef interface
#  endif
#  define interface struct

#  ifdef CONST_VTABLE
#    undef CONST_VTBL
#    define CONST_VTBL const
#    define MDECLARE_INTERFACE(iface) \
         typedef interface iface { const struct iface##Vtbl *lpVtbl; } iface; \
         typedef const struct iface##Vtbl iface##Vtbl; \
         const struct iface##Vtbl
#  else
#    undef CONST_VTBL
#    define CONST_VTBL
#    define MDECLARE_INTERFACE(iface) \
         typedef interface iface { struct iface##Vtbl *lpVtbl; } iface; \
         typedef struct iface##Vtbl iface##Vtbl; \
         struct iface##Vtbl
#  endif
#  define MDECLARE_INTERFACE_(iface,ibase) MDECLARE_INTERFACE(iface)

#  define MBEGIN_INTERFACE
#  define MEND_INTERFACE

#  define ICOM_MSVTABLE_COMPAT_DummyRTTIVALUE

#endif  /* __cplusplus && !CINTERFACE */

#define ICOM_DEFINE(iface,ibase) MDECLARE_INTERFACE_(iface,ibase) { iface##_METHODS };
#define ICOM_VTABLE(iface)       iface##Vtbl
#define ICOM_VFIELD(iface)       ICOM_VTABLE(iface)* lpVtbl
//#define ICOM_VTBL(iface)         (iface)->lpVtbl
#define ICOM_THIS(impl,iface)    impl* const pThis=(impl*)(iface)
#define ICOM_THIS_MULTI(impl,field,iface)  impl* const pThis=(impl*)((char*)(iface) - offsetof(impl,field))

#define ICOM_VFIELD_MULTI_NAME2(iface) ITF_##iface
#define ICOM_VFIELD_MULTI_NAME(iface) ICOM_VFIELD_MULTI_NAME2(iface)

#define ICOM_VFIELD_MULTI(iface) \
  iface ICOM_VFIELD_MULTI_NAME(iface)

#define ICOM_VFIELD_OFFSET(impltype, iface) \
  offsetof(impltype, ICOM_VFIELD_MULTI_NAME(iface))

// ICOM_OBJECT
#define GETINTERFACE(implobj, iface) \
  (&((implobj)->ICOM_VFIELD_MULTI_NAME(iface)))

#define ICOM_INIT_INTERFACE(implobj, ifacename, vtblname) \
  do { \
    (implobj)->ICOM_VFIELD_MULTI_NAME(ifacename).lpVtbl = &(vtblname); \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif
