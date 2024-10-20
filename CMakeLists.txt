cmake_minimum_required(VERSION 3.23)
project(ymsummorizer VERSION 0.0.1)
set(VENDOR "Arseny802")

message(STATUS "-- -- -- -- -- -- -- -- -- -- -- -- -- -- --")
message(STATUS "BUILD_TOOL_TYPE_NAME: ${BUILD_TOOL_TYPE_NAME}")
set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin/${BUILD_TOOL_TYPE_NAME}")

SET(BUILD_ARCHITECTURE "NOT_DEFINED")
if(${BUILD_TOOL_TYPE_NAME} MATCHES "^.*x86.*$")
  message(STATUS "Detected x86 architecture")
  SET(BUILD_ARCHITECTURE "x86")
elseif(${BUILD_TOOL_TYPE_NAME} MATCHES "^.*x64.*$")
  message(STATUS "Detected x64 architecture")
  SET(BUILD_ARCHITECTURE "x64")
endif()

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR} CACHE PATH "Single Directory for all")

cmake_policy(SET CMP0048 NEW)  # manages VERSION variables
cmake_policy(SET CMP0091 NEW)  # allows select the MSVC runtime library (MT, MD, etc)

# Set default compile flags for GCC
if (CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "GCC detected, adding compile flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fPIC")
  #set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
  #set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")
  #set(CMAKE_CXX_COMPILER_WORKS 1)
  #add_compile_options("-lstdc++11")
elseif (MSVC) # building on visual c++
  message(STATUS "MSVC detected, adding compile flags")
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
  ADD_DEFINITIONS("/EHsc")
endif (CMAKE_COMPILER_IS_GNUCXX)

#set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)
set(CPR_USE_SYSTEM_CURL ON)

enable_testing()

add_subdirectory(deps)
add_subdirectory(common)
add_subdirectory(storage)
add_subdirectory(ymapi)
add_subdirectory(tgbot)
add_subdirectory(service)

