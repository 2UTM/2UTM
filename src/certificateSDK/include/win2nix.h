/*************************************************************************
* Rutoken                                                                *
* Copyright (c) 2003-2023, Aktiv-Soft JSC. All rights reserved.          *
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
#include <windows.h>

typedef HANDLE mutex_t;
#else
#include <pthread.h>

#define uintptr_t pthread_t

typedef pthread_mutex_t mutex_t;
#endif

typedef struct {
	void (* func)(void*);
	void* arg;
} thread_args;

static
#ifdef _WIN32
unsigned __stdcall
#else
void*
#endif
wrapperRunner(void* arg){
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
	if (th == NULL) {
		return 1;
	}
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
	return !!rv;
}

static int createMutex(mutex_t* mutex)
{
#ifdef _WIN32
	*mutex = CreateMutex(NULL, FALSE, NULL);
	if (!*mutex) {
		return 1;
	}
#else
	if (pthread_mutex_init(mutex, NULL)) {
		return 1;
	}
#endif
	return 0;
}

static int destroyMutex(mutex_t* mutex)
{
#ifdef _WIN32
	if (!CloseHandle(*mutex)) {
		return 1;
	}
#else
	if (pthread_mutex_destroy(mutex)) {
		return 1;
	}
#endif
	return 0;
}

static int lockMutex(mutex_t* mutex)
{
#ifdef _WIN32
	if (WaitForSingleObject(*mutex, INFINITE) != WAIT_OBJECT_0) {
		return 1;
	}
#else
	if (pthread_mutex_lock(mutex)) {
		return 1;
	}
#endif
	return 0;
}

static int unlockMutex(mutex_t* mutex)
{
#ifdef _WIN32
	if (!ReleaseMutex(*mutex)) {
		return 1;
	}
#else
	if (pthread_mutex_unlock(mutex)) {
		return 1;
	}
#endif
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
