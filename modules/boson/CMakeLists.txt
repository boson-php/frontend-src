# --------------------------------------------------------------------------------------------------------
# Setup library
# --------------------------------------------------------------------------------------------------------

saucer_bindings_add_module(boson "BOSON_EXPORT")

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${MODULE_NAME} PUBLIC  "include")
target_include_directories(${MODULE_NAME} PRIVATE "include/saucer")

# --------------------------------------------------------------------------------------------------------
# Add Sources
# --------------------------------------------------------------------------------------------------------

if (saucer_backend STREQUAL "Default")
    if (WIN32)
        set(saucer_backend WebView2)
    elseif (APPLE)
        set(saucer_backend WebKit)
    else()
        set(saucer_backend WebKitGtk)
    endif()

    saucer_message(STATUS "Backend is 'Default', using ${saucer_backend}")
endif()

add_compile_definitions(BOSON_VERSION=0.6.0)
add_compile_definitions(SAUCER_BACKEND=${saucer_backend})

target_sources(${MODULE_NAME} PRIVATE
    "src/boson.cpp"
    "src/desktop_extensions.cpp"
)