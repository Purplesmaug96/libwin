cmake_minimum_required(VERSION 3.10)

project(libwin VERSION 1.0.0 DESCRIPTION "A windows shim." LANGUAGES C CXX)

add_library(libwin)

target_include_directories(libwin
    PRIVATE src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(NOT LIBWIN_HEADER_ONLY)
target_sources(libwin PRIVATE
	src/variables.c
)
else()
target_sources(libwin PRIVATE
	src/dummy.c
)
endif()

set_target_properties(libwin PROPERTIES OUTPUT_NAME win)

# ============================================================================
# Testing Configuration
# ============================================================================

enable_testing()

# Create test executable
add_executable(test_libwin test_libwin.c)

# Set C++ language for test
set_source_files_properties(test_libwin.c PROPERTIES LANGUAGE CXX)

# Include directories for the test
target_include_directories(test_libwin PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/doctest
)

# Link against libwin (header-only or compiled)
target_link_libraries(test_libwin PRIVATE libwin)

# Add test to CTest
add_test(NAME libwin_tests COMMAND test_libwin)

# Optional: Set verbose output for tests
set_tests_properties(libwin_tests PROPERTIES 
    ENVIRONMENT "DOCTEST_CONFIG_NO_COLORS=0"
)

# Custom target to run tests with verbose output
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -V
    DEPENDS test_libwin
)

# Print status
message(STATUS "libwin tests configured")
message(STATUS "  Test executable: test_libwin")
message(STATUS "  Build with: cmake --build . --target test_libwin")
message(STATUS "  Run with:   ctest --output-on-failure")
message(STATUS "  Or run:     ./test_libwin")
