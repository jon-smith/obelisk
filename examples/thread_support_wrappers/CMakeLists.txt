add_executable(thread_support_wrappers
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

include(${CMAKE_SOURCE_DIR}/cmake/obelisk-config-internal.cmake)

obelisk_config_internal(thread_support_wrappers)