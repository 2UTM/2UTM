/*************************************************************************
* Rutoken                                                                *
* Copyright (c) 2003-2021, Aktiv-Soft JSC. All rights reserved.          *
* Подробная информация:  http://www.rutoken.ru                           *
*------------------------------------------------------------------------*
* Данный файл содержит переопределение функций Windows для               *
* *nix-платформ                                                          *
*************************************************************************/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <process.h>
#else
#include <pthread.h>

#define uintptr_t pthread_t
#endif

typedef struct {
	void (*func)(void*);
	void* arg;
} thread_args;

static
#ifdef _WIN32
unsigned __stdcall
#else
void*
#endif
wrapperRunner(void* arg)
{
	thread_args* th = (thread_args*)arg;
	th->func(th->arg);
	free(th);
#ifdef _WIN32
	_endthreadex(0);
	return 0;
#else
	return NULL;
#endif
}

static int createThread(uintptr_t* thread, void (* funct)(void*), void* arg)
{
	thread_args* th = (thread_args*)malloc(sizeof(thread_args));
	th->arg = arg;
	th->func = funct;
#ifdef _WIN32
	*thread = _beginthreadex(NULL, 0, wrapperRunner, th, 0, NULL);
	if (*thread == 0)
#else
	if (pthread_create(thread, NULL, wrapperRunner, th))
#endif
	{
		free(th);
		return 1;
	}

	return 0;
}

static int joinThread(uintptr_t thread)
{
#ifdef _WIN32
	DWORD rv1 = WaitForSingleObject((HANDLE)thread, INFINITE);
	int rv = !CloseHandle((HANDLE)thread) || rv1;
#else
	int rv = pthread_join(thread, NULL);
#endif
	if (rv)
		return 1;
	return 0;
}

#if defined(__unix__) || defined(__APPLE__)

#include <dlfcn.h>

#include "wintypes.h"

typedef void* HMODULE;

static HMODULE LoadLibrary(const char* path)
{
	return dlopen(path, RTLD_NOW);
}

static BOOL FreeLibrary(HMODULE module)
{
	// return value is inverted in order to correspond to Windows behavior:
	return !dlclose(module);
}

static ptrdiff_t GetProcAddress(HMODULE module, const char* proc_name)
{
	return (ptrdiff_t)(dlsym(module, proc_name));
}

#endif // __unix__ || __APPLE__
