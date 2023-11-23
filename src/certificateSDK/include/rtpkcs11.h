/*******************************************************************
* Copyright (C) Aktiv Co. 2003-2017                                *
* rtpkcs11.h                                                       *
* Файл, включающий все символы для работы с библиотекой PKCS#11,   *
* а также расширения для Rutoken.                                  *
********************************************************************/

#ifndef __RTPKCS11_H__
#define __RTPKCS11_H__

/* Стандартный заголовочный файл с описанием всех типов и функций */
#include "cryptoki.h"

#ifdef _WIN32
#pragma pack(push, rtpkcs11, 1)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "rtpkcs11t.h"

#define __PASTE(x,y)      x##y

/* ==============================================================
 * Define the "extern" form of all the entry points.
 * ==============================================================
 */

#define CK_NEED_ARG_LIST  1
#define CK_PKCS11_FUNCTION_INFO(name) \
  extern CK_DECLARE_FUNCTION(CK_RV, name)

/* rtpkcs11f.h has all the information about the Cryptoki
 * extended function prototypes. */
#include "rtpkcs11f.h"

#undef CK_NEED_ARG_LIST
#undef CK_PKCS11_FUNCTION_INFO


/* ==============================================================
 * Define the typedef form of all the entry points.  That is, for
 * each Cryptoki function C_EX_XXX, define a type CK_C_XXX which
 * is a pointer to that kind of function.
 * ==============================================================
 */

#define CK_NEED_ARG_LIST  1
#define CK_PKCS11_FUNCTION_INFO(name) \
  typedef CK_DECLARE_FUNCTION_POINTER(CK_RV, __PASTE(CK_,name))

/* rtpkcs11f.h has all the information about the Cryptoki
 * extended function prototypes. */
#include "rtpkcs11f.h"

#undef CK_NEED_ARG_LIST
#undef CK_PKCS11_FUNCTION_INFO


/* ==============================================================
 * Define structed vector of entry points.
 * A CK_FUNCTION_LIST_EXTENDED
 * contains a CK_VERSION indicating a library's Cryptoki version
 * and then a whole slew of function pointers to the routines in
 * the library.  This type was declared, but not defined, in
 * rtpkcs11t.h.
 * ==============================================================
 */

#define CK_PKCS11_FUNCTION_INFO(name) \
  __PASTE(CK_,name) name;
  
struct CK_FUNCTION_LIST_EXTENDED {

  CK_VERSION    version;  /* Cryptoki version */

/* Pile all the function pointers into the
 * CK_FUNCTION_LIST_EXTENDED. */
/* rtpkcs11f.h has all the information about the Cryptoki
 * extended function prototypes. */
#include "rtpkcs11f.h"

};

#undef CK_PKCS11_FUNCTION_INFO

#undef __PASTE

#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#pragma pack(pop, rtpkcs11)
#endif

#endif /* __RTPKCS11_H__ */

