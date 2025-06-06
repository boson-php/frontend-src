cmake_minimum_required(VERSION 3.21)
project(boson LANGUAGES CXX VERSION 5.0.0)

set(COPYRIGHT "Copyright (c) Nesmeyanov Kirill/Community")
set(VENDOR "Boson")
set(DESCRIPTION "Boson frontend runtime binaries")
set(VERSION "0.3.0")
set(OUTPUT_NAME "libboson")

add_compile_definitions(BOSON_COPYRIGHT=${COPYRIGHT})
add_compile_definitions(BOSON_VENDOR=${VENDOR})
add_compile_definitions(BOSON_DESCRIPTION=${DESCRIPTION})
add_compile_definitions(BOSON_VERSION=${VERSION})

if(WIN32)
    set(OUTPUT_NAME "${OUTPUT_NAME}-windows")
elseif(APPLE)
    set(OUTPUT_NAME "${OUTPUT_NAME}-darwin")
else()
    set(OUTPUT_NAME "${OUTPUT_NAME}-linux")
endif()

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build universal binary" FORCE)
endif()

if(APPLE)
    set(OUTPUT_NAME "${OUTPUT_NAME}-universal")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(OUTPUT_NAME "${OUTPUT_NAME}-x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i[3-6]86|^x86$")
    set(OUTPUT_NAME "${OUTPUT_NAME}-x86")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    set(OUTPUT_NAME "${OUTPUT_NAME}-arm")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(OUTPUT_NAME "${OUTPUT_NAME}-aarch64")
else()
    set(OUTPUT_NAME "${OUTPUT_NAME}-unknown")
endif()


# --------------------------------------------------------------------------------------------------------
# CMake options
# --------------------------------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-subobject-linkage")
endif()

# --------------------------------------------------------------------------------------------------------
# Setup Dependencies
# --------------------------------------------------------------------------------------------------------

include(FetchContent)

set(saucer_static ON CACHE BOOL "Build a static library" FORCE)
set(saucer_modules ON CACHE BOOL "Enable module support" FORCE)
set(saucer_desktop ON CACHE BOOL "Enable support for the desktop module" FORCE)
set(glaze_ENABLE_AVX2 OFF CACHE BOOL "Disable AVX2 instructions set" FORCE)

set(ENV{GIT_TERMINAL_PROMPT} 0)

FetchContent_Declare(
    saucer
    GIT_REPOSITORY https://github.com/saucer/saucer
    GIT_TAG v6.0.1)
FetchContent_MakeAvailable(saucer)

FetchContent_Declare(
    saucer-bindings
    GIT_REPOSITORY https://github.com/saucer/bindings
    GIT_TAG v6.0.1)
FetchContent_MakeAvailable(saucer-bindings)

FetchContent_Declare(
    saucer-desktop
    GIT_REPOSITORY https://github.com/saucer/desktop
    GIT_TAG v3.0.0
)
FetchContent_MakeAvailable(saucer-desktop)

# --------------------------------------------------------------------------------------------------------
# Setup library
# --------------------------------------------------------------------------------------------------------

add_library(${PROJECT_NAME} SHARED "src/boson.cpp")

target_compile_definitions(${PROJECT_NAME} PRIVATE BOSON_BUILD)

target_link_libraries(${PROJECT_NAME} PRIVATE saucer::saucer)
target_link_libraries(${PROJECT_NAME} PRIVATE saucer::desktop)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".dll")
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".dylib")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".so")
endif()


if (PROJECT_IS_TOP_LEVEL AND NOT MSVC AND NOT CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic -Werror -pedantic -pedantic-errors -Wfatal-errors)
endif()

if (NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wno-unknown-warning-option -Wno-missing-field-initializers -Wno-cast-function-type)
endif()

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${saucer-bindings_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${saucer-bindings_SOURCE_DIR}/include/saucer>
    $<BUILD_INTERFACE:${saucer-bindings_SOURCE_DIR}/modules/desktop/include>
    $<BUILD_INTERFACE:${saucer-bindings_SOURCE_DIR}/modules/desktop/include/saucer>

    $<BUILD_INTERFACE:${saucer-bindings_SOURCE_DIR}/private>
)

target_sources(${PROJECT_NAME} PRIVATE
    ${saucer-bindings_SOURCE_DIR}/src/app.cpp
    ${saucer-bindings_SOURCE_DIR}/src/options.cpp
    ${saucer-bindings_SOURCE_DIR}/src/memory.cpp
    ${saucer-bindings_SOURCE_DIR}/src/icon.cpp
    ${saucer-bindings_SOURCE_DIR}/src/stash.cpp
    ${saucer-bindings_SOURCE_DIR}/src/script.cpp
    ${saucer-bindings_SOURCE_DIR}/src/scheme.cpp
    ${saucer-bindings_SOURCE_DIR}/src/navigation.cpp
    ${saucer-bindings_SOURCE_DIR}/src/preferences.cpp
    ${saucer-bindings_SOURCE_DIR}/src/window.cpp
    ${saucer-bindings_SOURCE_DIR}/src/webview.cpp

    ${saucer-bindings_SOURCE_DIR}/modules/desktop/src/desktop.cpp
)

