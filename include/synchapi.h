#pragma once

#include "libwin_msvcrt.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "windef.h"
#include "winnt.h"

typedef struct {
} CRITICAL_SECTION;
typedef CRITICAL_SECTION* LPCRITICAL_SECTION;

static inline void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection) {
	printf("Stubbed function InitializeCriticalSection called\n");
}

static inline void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection) {
	printf("Stubbed function DeleteCriticalSection called\n");
}

static inline void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection) {
	printf("Stubbed function EnterCriticalSection called\n");
}

static inline void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection) {
	printf("Stubbed function LeaveCriticalSection called\n");
}

static inline void Sleep(DWORD dwMilliseconds) {
	usleep(dwMilliseconds * 1000UL);
}

static inline DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds) {
	// Assuming hHandle is an libwin_struct_thread_handle
	return pthread_join(((libwin_struct_thread_handle*)hHandle)->thread, NULL) ? 0xffffffff : 0x00000000L; // i think WAIT_OBJECT_0 is for success??
}

#define EVENT_MODIFY_STATE 0

static inline HANDLE OpenEventA(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName) {
	printf("Stubbed function OpenEventA called\n");
	return NULL;
}

static inline BOOL SetEvent(HANDLE hEvent) {
	printf("Stubbed function SetEvent called\n");
	return FALSE;
}
