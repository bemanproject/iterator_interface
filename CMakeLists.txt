cmake_minimum_required(VERSION 3.10)

project(beman_iter_interface VERSION 0.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(FetchContent)
include(CompilerFeatureTest)

beman_iterator26_check_deducing_this(COMPILER_SUPPORTS_DEDUCING_THIS)

if(NOT COMPILER_SUPPORTS_DEDUCING_THIS)
  message(FATAL_ERROR "The selected compiler and flags lacks support C++23's deducing this, which is required to build this project. Try adding -DCMAKE_CXX_STANDARD=23 to your command line parameters and, failing that, upgrade your compiler.")
endif()

enable_testing()

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

add_subdirectory(extern)
add_subdirectory(src/beman/iterator_interface26)
add_subdirectory(examples)

include(GNUInstallDirs)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

install(
  EXPORT ${TARGETS_EXPORT_NAME}
  NAMESPACE ${CMAKE_PROJECT_NAME}
  DESTINATION ${INSTALL_CONFIGDIR}
  )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "cmake/Config.cmake.in"
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)
