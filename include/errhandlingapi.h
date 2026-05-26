#pragma once

#include "libwin_msvcrt.h"

#include <stdint.h>

#ifdef libwin_lasterror_local
#include "libwin_local_lasterror.h"
#else
extern uint32_t __windows_LastError;
#endif

static inline void SetLastError(uint32_t errorCode) {
	__windows_LastError = errorCode;
}

static inline uint32_t /* Should be DWORD, but not defined here */ GetLastError() {
	return __windows_LastError;
}
