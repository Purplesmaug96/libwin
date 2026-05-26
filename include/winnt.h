#pragma once

#include "libwin_msvcrt.h"

#include <stdint.h>
#include <pthread.h>

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

typedef struct {
	pthread_t thread;
} libwin_struct_thread_handle;

typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HBRUSH;

typedef struct {
} SECURITY_ATTRIBUTES;
typedef SECURITY_ATTRIBUTES* LPSECURITY_ATTRIBUTES;

typedef int32_t HRESULT;

typedef DWORD(__stdcall* LPTHREAD_START_ROUTINE)(void* lpThreadParameter);

#define FILE_ATTRIBUTE_READONLY             0x00000001
#define FILE_ATTRIBUTE_HIDDEN               0x00000002
#define FILE_ATTRIBUTE_SYSTEM               0x00000004
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020
#define FILE_ATTRIBUTE_DEVICE               0x00000040
#define FILE_ATTRIBUTE_NORMAL               0x00000080
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800
#define FILE_ATTRIBUTE_OFFLINE              0x00001000
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000
#define FILE_ATTRIBUTE_INTEGRITY_STREAM     0x00008000
#define FILE_ATTRIBUTE_VIRTUAL              0x00010000
#define FILE_ATTRIBUTE_NO_SCRUB_DATA        0x00020000
#define FILE_ATTRIBUTE_EA                   0x00040000
#define FILE_ATTRIBUTE_PINNED               0x00080000
#define FILE_ATTRIBUTE_UNPINNED             0x00100000
#define FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS 0x00400000

#define INVALID_FILE_ATTRIBUTES             ((DWORD)-1)

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
