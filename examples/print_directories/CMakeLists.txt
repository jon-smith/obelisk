add_executable(print_directories
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

include(${CMAKE_SOURCE_DIR}/cmake/obelisk-config-internal.cmake)

obelisk_config_internal(print_directories)