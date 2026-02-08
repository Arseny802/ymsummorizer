cmake_minimum_required(VERSION 3.23)
project(ymsummorizer.common VERSION ${CMAKE_PROJECT_VERSION})

find_package(magic_enum REQUIRED)
find_package(Boost REQUIRED)

add_library(${PROJECT_NAME} STATIC
  src/group.cpp
  src/playlist.cpp
  src/playlist_track.cpp
  src/user.cpp
  src/pch.cpp
)

target_precompile_headers(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/common/common_pch.h
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  hare
  magic_enum::magic_enum
  Boost::boost
  $<$<PLATFORM_ID:Windows>:ws2_32>
  $<$<PLATFORM_ID:Windows>:wsock32>
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

configure_file(version.h.in 
  ${PROJECT_SOURCE_DIR}/include/common/version.current.h
  NEWLINE_STYLE LF
)

execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files 
  ${PROJECT_SOURCE_DIR}/include/common/version.current.h
  ${PROJECT_SOURCE_DIR}/include/common/version.h
  RESULT_VARIABLE VersionChanged
)
if(VersionChanged EQUAL 0)
  message(STATUS "Version was NOT changed")
elseif(VersionChanged EQUAL 1)
  message(STATUS "Project version changed - updating version.h")
  file(REMOVE ${PROJECT_SOURCE_DIR}/include/common/version.h)
  file(RENAME 
    ${PROJECT_SOURCE_DIR}/include/common/version.current.h
    ${PROJECT_SOURCE_DIR}/include/common/version.h 
  )
else()
  message(WARNING "Error while comparing version.h files!")
endif()

