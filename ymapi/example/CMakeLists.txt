cmake_minimum_required(VERSION 3.23)
project(ymapi.example VERSION 0.0.1)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/pch.h
    src/pch.cpp)

target_precompile_headers(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

target_link_libraries(${PROJECT_NAME} PRIVATE ymapi)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
