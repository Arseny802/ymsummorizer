cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.tgbot VERSION ${CMAKE_PROJECT_VERSION})

find_package(tgbot CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)
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
    ymsummorizer.storage
    nlohmann_json::nlohmann_json
    Boost::boost
    tgbot::tgbot
  TEST_SOURCES_PATH 
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)
