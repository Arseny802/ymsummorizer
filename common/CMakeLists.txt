cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.common VERSION 0.0.1)

add_library(${PROJECT_NAME} STATIC

  src/pch.cpp
)

target_precompile_headers(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

target_link_libraries(${PROJECT_NAME} PUBLIC
  hare
  ws2_32
  wsock32
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

add_subdirectory(tests)
