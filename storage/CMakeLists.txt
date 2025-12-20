cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.storage VERSION 0.0.1)

find_package(Boost CONFIG REQUIRED)
find_package(SQLite3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB SQLite_SOURCES "sql/sqlite/*.sql")
cmrc_add_resource_library(${PROJECT_NAME}.resources
  NAMESPACE ymsummorizer::storage::sql_resources
  WHENCE sql
  ${SQLite_SOURCES}
)

add_library(${PROJECT_NAME} STATIC
  src/db/sqlite/manager.cpp
  src/db/cfg/manager.cpp
  src/db/fabric.cpp
  src/db/manager_base.cpp
  src/db_manager.cpp
  src/hare_storage.cpp
  src/pch.cpp
)

target_precompile_headers(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ymsummorizer.common
  ${PROJECT_NAME}.resources
  nlohmann_json::nlohmann_json
  Boost::boost
  SQLite::SQLite3
  $<$<PLATFORM_ID:Windows>:bcrypt>  # for boost UUID
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
