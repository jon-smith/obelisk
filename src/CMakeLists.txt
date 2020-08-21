add_library(obelisk
    ${CMAKE_CURRENT_SOURCE_DIR}/StringFuncs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PlatformSpecific.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StdThreadSupportWrappers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleAsyncTask.cpp
)

add_library(obelisk::obelisk ALIAS obelisk)

option(BUILD_SHARED_LIBS "Build shared library" ON)
include(GenerateExportHeader)
generate_export_header(obelisk
    EXPORT_MACRO_NAME OBELISK_API
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/obelisk/core/common.h
)

target_compile_features(obelisk PUBLIC cxx_std_17)
target_compile_options(obelisk PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -pipe -march=native
        $<$<CONFIG:Debug>:-O0 -Wall -Wextra>>
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Od /Wall /Zi>>
)

target_include_directories(obelisk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(obelisk PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/obelisk)

install(TARGETS obelisk
    EXPORT obelisk-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/obelisk
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/include/obelisk
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT obelisk-targets
    FILE obelisk-targets.cmake
    NAMESPACE obelisk::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/obelisk
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/obelisk-config-version.cmake
    VERSION ${OBELISK_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/obelisk-config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/obelisk-config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(
    FILES
        ${CMAKE_BINARY_DIR}/cmake/obelisk-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/obelisk-config-version.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT obelisk-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/obelisk-targets.cmake
    NAMESPACE obelisk::
)

export(PACKAGE obelisk)