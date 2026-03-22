# Файл для инициализации unit-тестов в проектах ymsummorizer
# Использование:
# include(common_tests)
# setup_project_tests(
#   NAME test_project_name
#   SOURCES test1.cpp test2.cpp
# )


option(BUILD_TESTS "Build tests" ON)
if (NOT BUILD_TESTS)
  return()
endif()

include(CTest)

# Найти пакет GTest
find_package(GTest REQUIRED)

function(setup_project_tests)
  set(options)
  set(oneValueArgs NAME)
  set(oneValueArgs SOURCES_PATH)
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # Если имя не передано, использовать PROJECT_NAME.test
  if(NOT ARG_NAME)
    set(ARG_NAME ${PROJECT_NAME}.test)
  endif()

  if(NOT ARG_SOURCES)
    if(NOT ARG_SOURCES_PATH)
      message(FATAL_ERROR "SOURCES or SOURCES_PATH argument is required for setup_project_tests")
    endif()
    file(GLOB_RECURSE ARG_SOURCES "${ARG_SOURCES_PATH}/*.cpp")
  endif()

  # Создать исполняемый файл тестов
  add_executable(${ARG_NAME} ${ARG_SOURCES})

  # Связать с библиотекой common и GTest
  target_link_libraries(${ARG_NAME} PRIVATE
    ${PROJECT_NAME}
    GTest::gmock_main
  )

  # Установить стандарт C++20
  set_target_properties(${ARG_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
  )

  target_compile_definitions(${ARG_NAME} PRIVATE UNIT_TEST)

  # Регистрация теста в CTest
  add_test(NAME ${ARG_NAME} COMMAND ${ARG_NAME})

  message(STATUS "Configured test: ${ARG_NAME}")


endfunction()
