/*************************************************************************
* Rutoken                                                                *
* Copyright (c) 2003-2023, Aktiv-Soft JSC. All rights reserved.          *
* Подробная информация:  http://www.rutoken.ru                           *
*------------------------------------------------------------------------*
* Данный файл содержит определение типов данных для *nix-платформ        *
*************************************************************************/

#pragma once

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef LONG
typedef long LONG;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef PBYTE
typedef BYTE* PBYTE;
#endif

#ifndef LPBYTE
typedef BYTE* LPBYTE;
#endif

#ifndef PVOID
typedef void* PVOID;
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif
