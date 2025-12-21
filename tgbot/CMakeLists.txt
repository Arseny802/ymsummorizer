cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.tgbot VERSION ${CMAKE_PROJECT_VERSION})

find_package(tgbot CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC
  src/handlers/group_playlist.cpp
  src/handlers/group_user.cpp
  src/handlers/group.cpp
  src/handlers/user.cpp
  src/helpers/user_autorization.cpp
  src/helpers/usrcmd_callbck_cache.cpp
  src/bot_impl.cpp
  src/ymsummorizer_bot.cpp
  src/hare_tgbot.cpp
  src/pch.cpp
)
  
target_precompile_headers(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ymsummorizer.common
  ymsummorizer.storage
  nlohmann_json::nlohmann_json
  Boost::boost
  tgbot::tgbot
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
