function(obelisk_config_internal target)
    target_compile_features(${target} PRIVATE cxx_std_17)
    target_compile_options(${target} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
            -pipe -march=native
            $<$<CONFIG:Debug>:-O0 -Wall -Wextra>>
        $<$<CXX_COMPILER_ID:MSVC>:
            $<$<CONFIG:Debug>:/Od /Wall /Zi>>
    )

    target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/../
    )

    target_link_libraries(${target} PRIVATE
        obelisk::obelisk
    )

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()