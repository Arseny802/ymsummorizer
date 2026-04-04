cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.ymapi VERSION ${CMAKE_PROJECT_VERSION})

find_package(ZLIB)
find_package(CURL)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_target(${PROJECT_NAME} STATIC
  INCLUDE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  SOURCES_PATH 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  PCH_PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
  USES_PUBLIC 
    ymsummorizer.common
    ZLIB::ZLIB
    CURL::libcurl
    cpr::cpr
    nlohmann_json::nlohmann_json
  TEST_SOURCES_PATH 
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

if (BUILD_EXAMPLES)
  add_subdirectory(example)
endif()
