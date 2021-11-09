cmake_minimum_required(VERSION 3.15)

project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES CXX
)

message(STATUS "Adding tests under ${CMAKE_PROJECT_NAME}Tests...")

foreach(file ${test_sources})
  string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.c)" "\\2" test_name ${file}) 
  add_executable(${test_name}_Tests ${file})

  # Setup code coverage if enabled
  if (${CMAKE_PROJECT_NAME}_ENABLE_CODE_COVERAGE)
      target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
      target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
      message(STATUS "Code coverage is enabled and provided with GCC.")
  endif()

  if(${CMAKE_PROJECT_NAME}_BUILD_EXECUTABLE)
    set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME}_LIB)
  else()
    set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME})
  endif()

  # Load the desired unit testing framework
  if(${CMAKE_PROJECT_NAME}_USE_CATCH2)
      find_package(Catch2 CONFIG REQUIRED)
    target_link_libraries(
      ${test_name}_Tests
      PUBLIC
        Catch2::Catch2
        ${${CMAKE_PROJECT_NAME}_TEST_LIB}
    )
  elseif(${CMAKE_PROJECT_NAME}_USE_GTEST)
    find_package(GTest REQUIRED)
    message(STATUS "The GTest is used.")
    target_link_libraries(
      ${test_name}_Tests
      PUBLIC
        GTest::GTest
        GTest::Main
        ${${CMAKE_PROJECT_NAME}_TEST_LIB}
      )
  else()
    find_package(CMocka CONFIG REQUIRED)
    target_link_libraries(
      ${test_name}_Tests
      PUBLIC
        cmocka
        ${${CMAKE_PROJECT_NAME}_TEST_LIB}
      )
  endif()

  # Add the unit tests
  add_test(
    NAME
      ${test_name}
    COMMAND
      ${test_name}_Tests
  )

endforeach()

message(STATUS "Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
