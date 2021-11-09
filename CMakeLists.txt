cmake_minimum_required(VERSION 3.15)

project(
  "LIS"
  VERSION 0.1.0
  LANGUAGES C)

include(cmake/StandardSettings.cmake)
include(cmake/StaticAnalyzers.cmake)

# Prevent building in the source directory
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n"
  )
endif()


add_compile_options(-g)
add_definitions(-DSERIAL)

include(cmake/SourcesAndHeaders.cmake)
message(${PROJECT_NAME}_BUILD_EXECUTABLE = ${${PROJECT_NAME}_BUILD_EXECUTABLE})
if(${PROJECT_NAME}_BUILD_EXECUTABLE)

  message(STATUS "Building executable...")
  add_executable(${PROJECT_NAME} ${exe_sources})

  if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
    add_library(${PROJECT_NAME}_LIB ${lis_headers} ${lis_sources})
  endif()

endif()

add_custom_command(
  TARGET ${PROJECT_NAME}_LIB
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/data/*
          $<TARGET_FILE_DIR:${PROJECT_NAME}_LIB>/test)

message(STATUS "Added all header and implementation files.\n")

include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

message(
  STATUS "Applied compiler warnings. Using standard ${CMAKE_CXX_STANDARD}.\n")

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_LIB)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:include>
                         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
  target_include_directories(
    ${PROJECT_NAME}_LIB
    PUBLIC $<INSTALL_INTERFACE:include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
endif()

message(STATUS "Finished setting up include directories.")


# Unit testing setup
if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
  enable_testing()
  message(STATUS "Build unit tests for the project.")
  add_subdirectory(test)
endif()
