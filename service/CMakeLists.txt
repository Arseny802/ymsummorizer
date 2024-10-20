cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.service VERSION 0.0.1)

#find_package(cpr CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(${PROJECT_NAME}

  src/hare_service.cpp
  src/main.cpp
  src/pch.cpp
)

target_precompile_headers(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ymsummorizer.common
  ymsummorizer.storage
  ymsummorizer.tgbot
  ymsummorizer.ymapi
)

target_include_directories(${PROJECT_NAME}
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

add_subdirectory(tests)
