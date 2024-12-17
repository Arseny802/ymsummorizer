cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.service VERSION 0.0.1)

add_executable(${PROJECT_NAME}
  src/handlers/group_playlist.cpp
  src/handlers/group_user.cpp
  src/handlers/group.cpp
  src/handlers/user.cpp
  src/bot_impl.cpp
  src/hare_service.cpp
  src/main.cpp
  src/pch.cpp
)

target_precompile_headers(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

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
