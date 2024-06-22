set(TARGET_LIBRARY "beman_iterator")

add_library("${TARGET_LIBRARY}" STATIC "")

target_sources(
  "${TARGET_LIBRARY}"
  PRIVATE
  iterator_interface.cpp
)

include(GNUInstallDirs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories("${TARGET_LIBRARY}" PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}>  # <prefix>/include/scratch
)

install(
  TARGETS "${TARGET_LIBRARY}"
  EXPORT ${TARGETS_EXPORT_NAME}1
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LOWER_PROJECT_NAME)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}
  FILES_MATCHING PATTERN "*.hpp"
)

target_link_libraries("${TARGET_LIBRARY}")

## Tests
add_executable(iterator_interface_test "")

target_sources(
  iterator_interface_test
  PRIVATE
  iterator_interface.t.cpp
)

target_link_libraries(iterator_interface_test "${TARGET_LIBRARY}")
target_link_libraries(iterator_interface_test gtest)
target_link_libraries(iterator_interface_test gtest_main)

include(GoogleTest)
gtest_discover_tests(iterator_interface_test)
