cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.service VERSION ${CMAKE_PROJECT_VERSION})

add_target(${PROJECT_NAME} EXECUTABLE
  SOURCES_PATH 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  PCH_PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
  USES_PUBLIC 
    ymsummorizer.common
    ymsummorizer.storage
    ymsummorizer.tgbot
    ymsummorizer.ymapi
  TEST_SOURCES_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
  TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../storage/mocs/dbmoc.hpp
  TEST_USES
    ymsummorizer.storage
  TEST_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/../storage/mocs/
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  OUTPUT_NAME "ymsummorizer-${PROJECT_VERSION}" 
)
