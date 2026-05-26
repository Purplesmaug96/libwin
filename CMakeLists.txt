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
