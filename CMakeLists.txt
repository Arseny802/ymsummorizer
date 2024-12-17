cmake_minimum_required(VERSION 3.23)
project(ymsummorizer VERSION 0.0.1)

add_subdirectory(deps)
add_subdirectory(common)
add_subdirectory(storage)
add_subdirectory(ymapi)
add_subdirectory(tgbot)
add_subdirectory(service)

include(InstallRequiredSystemLibraries)
install(
  TARGETS ymsummorizer.service
)
