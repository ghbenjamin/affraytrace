# Globals

cmake_minimum_required(VERSION 3.30)
project(affraytrace)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")


# Build library

include_directories( include )

file(GLOB_RECURSE CPP_SOURCE_LIST "src/*.cpp")
list(FILTER CPP_SOURCE_LIST EXCLUDE REGEX ".*src/main\\.cpp$")

add_library( affraytrace_lib STATIC ${CPP_SOURCE_LIST} )


# Third party libraries: stb_image, tinyxml, cxxopts

find_package(Stb REQUIRED)
target_include_directories(affraytrace_lib PRIVATE ${Stb_INCLUDE_DIR})

find_package(tinyxml2 CONFIG REQUIRED)
target_link_libraries(affraytrace_lib PRIVATE tinyxml2::tinyxml2)


# Build main executable

include_directories(include)
add_executable(affraytrace "src/main.cpp" )

target_link_libraries( affraytrace PRIVATE affraytrace_lib  )

find_package(cxxopts CONFIG REQUIRED)
target_link_libraries(affraytrace PRIVATE cxxopts::cxxopts)


# Build tests

file(GLOB_RECURSE TEST_SOURCE_LIST "test/*.cpp")
add_executable(affraytrace_test ${TEST_SOURCE_LIST})

include_directories( include )

find_package(Catch2 REQUIRED)
target_link_libraries(affraytrace_test PRIVATE affraytrace_lib )
target_link_libraries(affraytrace_test PRIVATE Catch2::Catch2WithMain )

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)
include(Catch)
catch_discover_tests(affraytrace_test)