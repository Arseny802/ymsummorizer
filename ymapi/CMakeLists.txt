cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.ymapi VERSION ${CMAKE_PROJECT_VERSION})

find_package(ZLIB)
find_package(CURL)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC
  src/client.cpp
  src/playlist.cpp
  src/pch.cpp
  src/ymapi.cpp
)

target_precompile_headers(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ymsummorizer.common
  ZLIB::ZLIB
  CURL::libcurl
  cpr::cpr
  nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src")

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

add_subdirectory(example)
add_subdirectory(tests)
