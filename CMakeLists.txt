cmake_minimum_required(VERSION 3.23)
project(ymsummorizer VERSION 0.0.1)

option(BUILD_TESTS "Build project tests" ON)
option(RUN_TESTS "Run project tests" ON)
option(ENABLE_COVERAGE "Enable code coverage reporting" ON)
option(BUILD_EXAMPLES "Build project examples" ON)

include(common/common.cmake)

add_subdirectory(deps)
add_subdirectory(common)
add_subdirectory(storage)
add_subdirectory(ymapi)
add_subdirectory(tgbot)
add_subdirectory(service)

set(INSTALL_DESTINATION ymsummorizer)
install(
  TARGETS ymsummorizer.service
  DESTINATION ${INSTALL_DESTINATION}
)
