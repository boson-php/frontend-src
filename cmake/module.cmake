cmake_policy(SET CMP0140 NEW)

function(saucer_bindings_add_module NAME MACRO)
    set(MODULE_NAME  "saucer-bindings-${NAME}")
    set(MODULE_ALIAS "saucer::bindings::${NAME}")

    if ("${NAME}" IN_LIST saucer_bindings_inline_modules)
        add_library(${MODULE_NAME} OBJECT)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${MODULE_NAME})
    else()
        add_library(${MODULE_NAME} MODULE)
    endif()

    add_library(${MODULE_ALIAS} ALIAS ${MODULE_NAME})
    target_link_libraries(${MODULE_NAME} PRIVATE saucer::private)

    target_compile_features(${MODULE_NAME} PRIVATE cxx_std_23)
    set_target_properties(${MODULE_NAME} PROPERTIES CXX_STANDARD 23 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

    saucer_bindings_hide_symbols(${MODULE_NAME})
    saucer_bindings_export(${MODULE_NAME} ${MACRO})

    get_target_property(priv_includes saucer::bindings INCLUDE_DIRECTORIES)
    target_include_directories(${MODULE_NAME} PRIVATE ${priv_includes})

    return(PROPAGATE MODULE_NAME)
endfunction()
