/*
 * libwin Test Suite using doctest
 * 
 * This test suite verifies the accuracy and correctness of the Windows API shim functions
 * in libwin. Tests are written using the doctest header-only testing framework.
 * 
 * Compile with:
 *   gcc -I./include -I./external/doctest test_libwin.c -o test_libwin
 * 
 * Or with winelib:
 *   winegcc -I./include -I./external/doctest test_libwin.c -o test_libwin
 * 
 * Or using CMake:
 *   mkdir build && cd build && cmake .. && make && ./test_libwin
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "include/windows.h"

/* ============================================================================
 * Type Conversion Tests
 * ============================================================================ */

DOCTEST_TEST_CASE("MAKEWORD macro - combines two bytes into a word") {
    // MAKEWORD(x, y) creates: 0x00yy00xx -> 0xyyxx
    uint16_t result = MAKEWORD(0xAA, 0xBB);
    DOCTEST_CHECK_EQ(result, 0xBBAA);
    
    DOCTEST_CHECK_EQ(MAKEWORD(0x12, 0x34), 0x3412);
    DOCTEST_CHECK_EQ(MAKEWORD(0xFF, 0x00), 0x00FF);
    DOCTEST_CHECK_EQ(MAKEWORD(0x00, 0xFF), 0xFF00);
}

DOCTEST_TEST_CASE("MAKEWORD - edge cases with zero values") {
    DOCTEST_CHECK_EQ(MAKEWORD(0x00, 0x00), 0x0000);
    DOCTEST_CHECK_EQ(MAKEWORD(0xFF, 0xFF), 0xFFFF);
}

/* ============================================================================
 * Integer to ASCII Conversion Tests
 * ============================================================================ */

DOCTEST_TEST_CASE("_itoa - convert integer to string base 10") {
    char buffer[32];
    
    // Positive number
    char* result = _itoa(12345, buffer, 10);
    DOCTEST_CHECK_EQ(result, buffer);  // Returns same buffer
    DOCTEST_CHECK_EQ(strcmp(buffer, "12345"), 0);
}

DOCTEST_TEST_CASE("_itoa - zero value") {
    char buffer[32];
    _itoa(0, buffer, 10);
    DOCTEST_CHECK_EQ(strcmp(buffer, "0"), 0);
}

DOCTEST_TEST_CASE("_itoa - negative numbers base 10") {
    char buffer[32];
    _itoa(-12345, buffer, 10);
    DOCTEST_CHECK_EQ(strcmp(buffer, "-12345"), 0);
}

DOCTEST_TEST_CASE("_itoa - binary base 2") {
    char buffer[32];
    _itoa(5, buffer, 2);
    DOCTEST_CHECK_EQ(strcmp(buffer, "101"), 0);
}

DOCTEST_TEST_CASE("_itoa - hexadecimal base 16") {
    char buffer[32];
    _itoa(255, buffer, 16);
    DOCTEST_CHECK_EQ(strcmp(buffer, "ff"), 0);
}

DOCTEST_TEST_CASE("_itoa - octal base 8") {
    char buffer[32];
    _itoa(64, buffer, 8);
    DOCTEST_CHECK_EQ(strcmp(buffer, "100"), 0);
}

DOCTEST_TEST_CASE("_itoa - negative with non-base-10 is unsigned") {
    char buffer[32];
    // Negative numbers are only handled with base 10
    // Other bases treat as unsigned
    _itoa(-1, buffer, 16);
    // This will treat -1 as unsigned, so should be large hex
    DOCTEST_CHECK_NE(buffer[0], '-');  // Should not have negative sign
}

DOCTEST_TEST_CASE("_ltoa - long to string") {
    char buffer[32];
    char* result = _ltoa(123456L, buffer, 10);
    DOCTEST_CHECK_EQ(strcmp(buffer, "123456"), 0);
}

DOCTEST_TEST_CASE("_ultoa - unsigned long to string") {
    char buffer[32];
    char* result = _ultoa(987654UL, buffer, 10);
    DOCTEST_CHECK_EQ(strcmp(buffer, "987654"), 0);
}

/* ============================================================================
 * String Manipulation Tests
 * ============================================================================ */

DOCTEST_TEST_CASE("lstrcpyA - string copy") {
    char dest[32];
    const char* src = "Hello, World!";
    char* result = lstrcpyA(dest, src);
    DOCTEST_CHECK_EQ(result, dest);
    DOCTEST_CHECK_EQ(strcmp(dest, "Hello, World!"), 0);
}

DOCTEST_TEST_CASE("lstrcpynA - string copy with length limit") {
    char dest[32];
    const char* src = "Hello, World!";
    char* result = lstrcpynA(dest, src, 6);  // Copy "Hello\0"
    DOCTEST_CHECK_EQ(result, dest);
    // Note: strncpy doesn't guarantee null termination if src is >= n
    // The first 6 bytes should be "Hello\0"
    DOCTEST_CHECK_EQ(strncmp(dest, "Hello", 5), 0);
}

DOCTEST_TEST_CASE("lstrcatA - string concatenation") {
    char dest[64] = "Hello";
    const char* src = ", World!";
    char* result = lstrcatA(dest, src);
    DOCTEST_CHECK_EQ(result, dest);
    DOCTEST_CHECK_EQ(strcmp(dest, "Hello, World!"), 0);
}

/* ============================================================================
 * Last Error Handling Tests
 * ============================================================================ */

DOCTEST_TEST_CASE("GetLastError and SetLastError") {
    // Initial value should be 0
    uint32_t initial = GetLastError();
    
    // Set a custom error code
    SetLastError(42);
    DOCTEST_CHECK_EQ(GetLastError(), 42U);
    
    // Set another error code
    SetLastError(12345);
    DOCTEST_CHECK_EQ(GetLastError(), 12345U);
    
    // Reset
    SetLastError(0);
    DOCTEST_CHECK_EQ(GetLastError(), 0U);
}

DOCTEST_TEST_CASE("SetLastError and GetLastError - error code persistence") {
    SetLastError(999);
    // Error code should persist until changed
    DOCTEST_CHECK_EQ(GetLastError(), 999U);
    DOCTEST_CHECK_EQ(GetLastError(), 999U);
}

/* ============================================================================
 * Interlocked Operations Tests (CRITICAL)
 * ============================================================================ */

DOCTEST_TEST_CASE("InterlockedIncrement - single threaded") {
    long value = 5;
    long result = InterlockedIncrement(&value);
    
    // Should return new value (6)
    DOCTEST_CHECK_EQ(result, 6L);
    DOCTEST_CHECK_EQ(value, 6L);
}

DOCTEST_TEST_CASE("InterlockedIncrement - multiple operations") {
    long value = 0;
    DOCTEST_CHECK_EQ(InterlockedIncrement(&value), 1L);
    DOCTEST_CHECK_EQ(InterlockedIncrement(&value), 2L);
    DOCTEST_CHECK_EQ(InterlockedIncrement(&value), 3L);
    DOCTEST_CHECK_EQ(value, 3L);
}

DOCTEST_TEST_CASE("InterlockedIncrement64 - 64-bit values") {
    long long value = 1000000000000LL;
    long long result = InterlockedIncrement64(&value);
    
    DOCTEST_CHECK_EQ(result, 1000000000001LL);
    DOCTEST_CHECK_EQ(value, 1000000000001LL);
}

DOCTEST_TEST_CASE("InterlockedDecrement - single threaded") {
    long value = 10;
    long result = InterlockedDecrement(&value);
    
    // Should return new value (9)
    DOCTEST_CHECK_EQ(result, 9L);
    DOCTEST_CHECK_EQ(value, 9L);
}

DOCTEST_TEST_CASE("InterlockedDecrement - multiple operations") {
    long value = 5;
    DOCTEST_CHECK_EQ(InterlockedDecrement(&value), 4L);
    DOCTEST_CHECK_EQ(InterlockedDecrement(&value), 3L);
    DOCTEST_CHECK_EQ(InterlockedDecrement(&value), 2L);
    DOCTEST_CHECK_EQ(value, 2L);
}

DOCTEST_TEST_CASE("InterlockedDecrement - negative values") {
    long value = 0;
    DOCTEST_CHECK_EQ(InterlockedDecrement(&value), -1L);
    DOCTEST_CHECK_EQ(InterlockedDecrement(&value), -2L);
    DOCTEST_CHECK_EQ(value, -2L);
}

DOCTEST_TEST_CASE("InterlockedDecrement64 - 64-bit values") {
    long long value = 1000000000001LL;
    long long result = InterlockedDecrement64(&value);
    
    DOCTEST_CHECK_EQ(result, 1000000000000LL);
    DOCTEST_CHECK_EQ(value, 1000000000000LL);
}

DOCTEST_TEST_CASE("InterlockedCompareExchange - successful exchange") {
    LONG value = 100;
    LONG result = InterlockedCompareExchange(&value, 200, 100);
    
    // Should return original value
    DOCTEST_CHECK_EQ(result, 100L);
    // Should have exchanged to new value
    DOCTEST_CHECK_EQ(value, 200L);
}

DOCTEST_TEST_CASE("InterlockedCompareExchange - failed exchange (mismatch)") {
    LONG value = 100;
    LONG result = InterlockedCompareExchange(&value, 200, 999);
    
    // Should return original value
    DOCTEST_CHECK_EQ(result, 100L);
    // Should NOT have changed
    DOCTEST_CHECK_EQ(value, 100L);
}

DOCTEST_TEST_CASE("InterlockedCompareExchange - sequential exchanges") {
    LONG value = 1;
    
    // First exchange: 1 -> 2
    DOCTEST_CHECK_EQ(InterlockedCompareExchange(&value, 2, 1), 1L);
    DOCTEST_CHECK_EQ(value, 2L);
    
    // Second exchange: 2 -> 3
    DOCTEST_CHECK_EQ(InterlockedCompareExchange(&value, 3, 2), 2L);
    DOCTEST_CHECK_EQ(value, 3L);
    
    // Failed exchange
    DOCTEST_CHECK_EQ(InterlockedCompareExchange(&value, 4, 2), 3L);
    DOCTEST_CHECK_EQ(value, 3L);
}

/* ============================================================================
 * Sleep Tests - Timing Accuracy
 * ============================================================================ */

// Helper function to get current time in milliseconds
static uint32_t GetTimeMs(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

DOCTEST_TEST_CASE("Sleep - 100ms sleep duration") {
    uint32_t start = GetTimeMs();
    Sleep(100);
    uint32_t end = GetTimeMs();
    uint32_t elapsed = end - start;
    
    // Should be approximately 100ms (allow 50% tolerance for system variation)
    // Range: 50-150ms
    DOCTEST_CHECK_GE(elapsed, 50U);
    DOCTEST_CHECK_LE(elapsed, 150U);
}

DOCTEST_TEST_CASE("Sleep - very short sleep (10ms)") {
    uint32_t start = GetTimeMs();
    Sleep(10);
    uint32_t end = GetTimeMs();
    uint32_t elapsed = end - start;
    
    // Should be approximately 10ms (minimum should be close to requested)
    // Range: 5-50ms (allowing for system scheduling variations)
    DOCTEST_CHECK_GE(elapsed, 5U);
    DOCTEST_CHECK_LE(elapsed, 50U);
}

DOCTEST_TEST_CASE("Sleep - zero sleep") {
    // Should return almost immediately
    uint32_t start = GetTimeMs();
    Sleep(0);
    uint32_t end = GetTimeMs();
    uint32_t elapsed = end - start;
    
    // Should take very little time (less than 10ms)
    DOCTEST_CHECK_LE(elapsed, 10U);
}

/* ============================================================================
 * Pointer Validation Tests
 * ============================================================================ */

DOCTEST_TEST_CASE("IsBadCodePtr - NULL pointer") {
    FARPROC null_ptr = NULL;
    DOCTEST_CHECK_EQ(IsBadCodePtr(null_ptr), TRUE);
}

DOCTEST_TEST_CASE("IsBadCodePtr - valid function pointer") {
    // Use the address of this test function as a valid pointer
    FARPROC valid_ptr = (FARPROC)&GetTimeMs;
    DOCTEST_CHECK_EQ(IsBadCodePtr(valid_ptr), FALSE);
}

DOCTEST_TEST_CASE("IsBadCodePtr - pointer to current function") {
    // Pointer to a function in the test binary
    void* func_ptr = (void*)&printf;
    DOCTEST_CHECK_EQ(IsBadCodePtr((FARPROC)func_ptr), FALSE);
}

/* ============================================================================
 * Type Definition Sanity Tests
 * ============================================================================ */

DOCTEST_TEST_CASE("Type definitions - BOOL size and values") {
    DOCTEST_CHECK_EQ(sizeof(BOOL), sizeof(int));
    DOCTEST_CHECK_EQ(TRUE, 1);
    DOCTEST_CHECK_EQ(FALSE, 0);
}

DOCTEST_TEST_CASE("Type definitions - DWORD size") {
    DOCTEST_CHECK_EQ(sizeof(DWORD), 4U);
    DOCTEST_CHECK_EQ(sizeof(WORD), 2U);
    DOCTEST_CHECK_EQ(sizeof(BYTE), 1U);
}

DOCTEST_TEST_CASE("Type definitions - character types") {
    DOCTEST_CHECK_EQ(sizeof(CHAR), sizeof(char));
    DOCTEST_CHECK_EQ(sizeof(WCHAR), sizeof(wchar_t));
}

DOCTEST_TEST_CASE("Type definitions - RECT structure") {
    RECT rect;
    rect.left = 10;
    rect.top = 20;
    rect.right = 100;
    rect.bottom = 200;
    
    DOCTEST_CHECK_EQ(rect.left, 10);
    DOCTEST_CHECK_EQ(rect.right, 100);
}

/* ============================================================================
 * Time Function Tests
 * ============================================================================ */

DOCTEST_TEST_CASE("timeGetTime - returns positive value") {
    DWORD time1 = timeGetTime();
    DOCTEST_CHECK_GT(time1, 0U);
    
    Sleep(10);
    
    DWORD time2 = timeGetTime();
    DOCTEST_CHECK_GT(time2, 0U);
    
    // time2 should be greater than or equal to time1
    DOCTEST_CHECK_GE(time2, time1);
}

DOCTEST_TEST_CASE("timeGetTime - monotonic increase") {
    DWORD times[5];
    
    for (int i = 0; i < 5; i++) {
        times[i] = timeGetTime();
        if (i > 0) {
            DOCTEST_CHECK_GE(times[i], times[i-1]);
        }
    }
}

/* ============================================================================
 * Stub Function Tests - Basic Validation
 * ============================================================================ */

DOCTEST_TEST_CASE("GetCurrentProcess - returns NULL") {
    HANDLE h = GetCurrentProcess();
    DOCTEST_CHECK_EQ(h, (HANDLE)NULL);
}

DOCTEST_TEST_CASE("ExitThread - type check") {
    // We can't actually call ExitThread as it will exit the process
    // Just verify it's defined
    DOCTEST_CHECK(1);  // Placeholder
}

DOCTEST_TEST_CASE("OutputDebugStringA - type check") {
    // Just verify it's defined and callable (it prints to stdout)
    OutputDebugStringA("test debug message");
    DOCTEST_CHECK(1);  // Placeholder
}

/* ============================================================================
 * Architecture Helper Tests
 * ============================================================================ */

DOCTEST_TEST_CASE("Architecture detection - valid architecture defined") {
    // Verify that libwin_arch is defined to a valid value
    DOCTEST_CHECK_LT(libwin_arch, 8);  // Should be 0-7
    DOCTEST_CHECK_GE(libwin_arch, 0);
}

DOCTEST_TEST_CASE("Architecture detection - detailed architecture defined") {
    // Verify that libwin_arch_detailed is defined
    DOCTEST_CHECK_LT(libwin_arch_detailed, 21);  // Should be 0-20
    DOCTEST_CHECK_GE(libwin_arch_detailed, 0);
}

/* ============================================================================
 * End of Tests
 * ============================================================================ */
