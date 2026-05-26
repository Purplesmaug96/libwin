#pragma once

#include "libwin_msvcrt.h"

#include <stdio.h>

#define __debugbreak() printf("__debugbreak\n")

static inline void libwin_OutputDebugStringA(const char* str, const char* file, const int line) {
	printf("OutputDebugStringA (%s:%d): %s\n", file, line, str);
}

#define OutputDebugStringA(str) libwin_OutputDebugStringA(str, __FILE__, __LINE__)
