cmake_minimum_required(VERSION 3.21)
project(saucer-bindings LANGUAGES CXX VERSION 8.0.2)

set(COPYRIGHT "Copyright (c) Nesmeyanov Kirill/Community")
set(VENDOR "Boson")
set(DESCRIPTION "Boson frontend runtime binaries")
set(VERSION "0.5.0")
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

# +-------------------------------------------------------------------------------------------------------+
# | CMake options                                                                                         |
# +-------------------------------------------------------------------------------------------------------+

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# +-------------------------------------------------------------------------------------------------------+
# | Setup library                                                                                         |
# +-------------------------------------------------------------------------------------------------------+

add_library(${PROJECT_NAME} SHARED)
add_library(saucer::bindings ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    PREFIX ""
    OUTPUT_NAME ${OUTPUT_NAME}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

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
        -Wno-unknown-warning-option -Wno-missing-field-initializers -Wno-cast-function-type -Wno-sign-compare)
endif()

# +-------------------------------------------------------------------------------------------------------+
# | Export header                                                                                         |
# +-------------------------------------------------------------------------------------------------------+

include("cmake/hide.cmake")
include("cmake/export.cmake")

saucer_bindings_hide_symbols(${PROJECT_NAME})
saucer_bindings_export(${PROJECT_NAME} "SAUCER_EXPORT")

# +-------------------------------------------------------------------------------------------------------+
# | Setup version header                                                                                  |
# +-------------------------------------------------------------------------------------------------------+

configure_file("template/version.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/private/version.hpp")

# +-------------------------------------------------------------------------------------------------------+
# | Include directories                                                                                   |
# +-------------------------------------------------------------------------------------------------------+

target_include_directories(${PROJECT_NAME} PUBLIC  "include")
target_include_directories(${PROJECT_NAME} PRIVATE "include/saucer" "private")

# +-------------------------------------------------------------------------------------------------------+
# | Add Sources                                                                                           |
# +-------------------------------------------------------------------------------------------------------+

target_sources(${PROJECT_NAME} PRIVATE
    "src/boson.cpp"

    "src/app.cpp"
    "src/window.cpp"
    "src/webview.cpp"

    "src/url.cpp"
    "src/icon.cpp"
    "src/stash.cpp"

    "src/scheme.cpp"
    "src/permission.cpp"
    "src/navigation.cpp"
)

# +-------------------------------------------------------------------------------------------------------+
# | Setup Dependencies                                                                                    |
# +-------------------------------------------------------------------------------------------------------+

include("cmake/cpm.cmake")

CPMFindPackage(
    NAME           saucer
    VERSION        8.0.2
    GIT_REPOSITORY "https://github.com/saucer/saucer"
    OPTIONS        "saucer_static ON" "CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON"
)

target_link_libraries(${PROJECT_NAME} PUBLIC saucer::saucer saucer::private)

CPMFindPackage(
    NAME           saucer-desktop
    VERSION        4.2.0
    GIT_REPOSITORY "https://github.com/saucer/desktop"
)

target_link_libraries(${PROJECT_NAME} PUBLIC saucer::desktop)

CPMFindPackage(
    NAME           saucer-loop
    VERSION        2.1.0
    GIT_REPOSITORY "https://github.com/saucer/loop"
)

target_link_libraries(${PROJECT_NAME} PUBLIC saucer::loop)

# +-------------------------------------------------------------------------------------------------------+
# | Setup Natives                                                                                         |
# +-------------------------------------------------------------------------------------------------------+

if (saucer_backend STREQUAL "WebKit")
    enable_language(OBJCXX)
    configure_file("src/native.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/native.mm" COPYONLY)
    target_sources(${PROJECT_NAME} PRIVATE "src/native.mm")
else()
    target_sources(${PROJECT_NAME} PRIVATE "src/native.cpp")
endif()

# +-------------------------------------------------------------------------------------------------------+
# | Setup Modules                                                                                         |
# +-------------------------------------------------------------------------------------------------------+

target_include_directories(${PROJECT_NAME}
    PRIVATE "modules/desktop/include/saucer"
    PUBLIC "modules/desktop/include"

    PRIVATE "modules/loop/include/saucer"
    PUBLIC "modules/loop/include"
)

target_sources(${PROJECT_NAME} PRIVATE
    "modules/desktop/src/desktop.cpp"
    "modules/loop/src/loop.cpp"
)