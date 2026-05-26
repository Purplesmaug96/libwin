#pragma once

#include "libwin_msvcrt.h"

#include <stddef.h>

#define CONTAINING_RECORD(address, type, field) ((type*)(address - offsetof(type, field)))
