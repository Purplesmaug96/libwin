#pragma once

#include "libwin_msvcrt.h"

#include <stdint.h>

#include "windef.h"

typedef union {
	//   struct {
	//     DWORD LowPart;
	//     LONG  HighPart;
	//   } DUMMYSTRUCTNAME;
	struct {
		DWORD LowPart;
		LONG HighPart;
	} u;
	long long QuadPart;
} LARGE_INTEGER;

typedef void* HANDLE;

// Quite possible the wrong place
typedef HANDLE HWND;
typedef HANDLE HRGN;

typedef struct {
	char* name;
	void* dlHandle;
} libwin_struct_HMODULE;
typedef libwin_struct_HMODULE* HMODULE;

typedef HANDLE HDC; // Device Context
typedef HANDLE HINSTANCE;
typedef HANDLE HGDIOBJ;

typedef struct libwin_struct_HKEY {
	char* path;
	char** pathSeperated; // List of folders (example: ["HKCU", "SOFTWARE", "Blizzard Entertainment"])
	char* name;
	int type;
	uint32_t value;
	libwin_struct_HKEY* parent = NULL;
}* HKEY;

typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HBRUSH;

typedef struct {
} SECURITY_ATTRIBUTES;
typedef SECURITY_ATTRIBUTES* LPSECURITY_ATTRIBUTES;

typedef int32_t HRESULT;

typedef DWORD(__stdcall* LPTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);

static inline long InterlockedIncrement(long volatile* Addend) {
	#ifdef __GNUC__
	return __sync_add_and_fetch(Addend, 1);
	#else
	// Fallback for non-GCC compilers (not thread-safe)
	return ++(*Addend);
	#endif
}

static inline long long InterlockedIncrement64(long long volatile* Addend) {
	#ifdef __GNUC__
	return __sync_add_and_fetch(Addend, 1);
	#else
	// Fallback for non-GCC compilers (not thread-safe)
	return ++(*Addend);
	#endif
}

static inline long InterlockedDecrement(long volatile* Addend) {
	#ifdef __GNUC__
	return __sync_sub_and_fetch(Addend, 1);
	#else
	// Fallback for non-GCC compilers (not thread-safe)
	return --(*Addend);
	#endif
}

static inline long long InterlockedDecrement64(long long volatile* Addend) {
	#ifdef __GNUC__
	return __sync_sub_and_fetch(Addend, 1);
	#else
	// Fallback for non-GCC compilers (not thread-safe)
	return --(*Addend);
	#endif
}

// InterlockedCompareExchange taken from this issue: https://github.com/itsmattkc/dotnet9x/issues/19
// Atomic compare and exchange for thread-safe operations

static inline LONG InterlockedCompareExchange(LONG volatile* dest, LONG xchg, LONG compare) {
	#ifdef __GNUC__
	// Use GCC atomic builtin for thread-safe operation
	return __sync_val_compare_and_swap(dest, compare, xchg);
	#else
	// Fallback for non-GCC compilers (not thread-safe)
	LONG temp = *dest;
	if (compare == *dest) {
		*dest = xchg;
	}
	return temp;
	#endif
}
