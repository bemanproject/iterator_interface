add_library(beman.iterator_interface26)
add_library(beman::iterator_interface26 ALIAS beman.iterator_interface26)

target_sources(
  beman.iterator_interface26
  PRIVATE
  iterator_interface.cpp
)

include(GNUInstallDirs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(beman.iterator_interface26 PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}>  # <prefix>/include/scratch
)

install(
  TARGETS beman.iterator_interface26
  EXPORT ${TARGETS_EXPORT_NAME}1
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LOWER_PROJECT_NAME)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}
  FILES_MATCHING PATTERN "*.hpp"
)

target_link_libraries(beman.iterator_interface26)

## Tests
if(BEMAN_ITERATOR_INTERFACE26_ENABLE_TESTING)
    add_executable(iterator_interface_test "")

    target_sources(
      iterator_interface_test
      PRIVATE
      iterator_interface.t.cpp
    )

    target_link_libraries(iterator_interface_test beman.iterator_interface26)
    target_link_libraries(iterator_interface_test GTest::gtest)
    target_link_libraries(iterator_interface_test GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(iterator_interface_test)
endif()
