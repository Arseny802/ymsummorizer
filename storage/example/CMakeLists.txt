cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.storage.example VERSION 0.0.1)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/pch.cpp
)

target_precompile_headers(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

target_link_libraries(${PROJECT_NAME} PRIVATE ymsummorizer.storage)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)
