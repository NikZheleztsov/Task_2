cmake_minimum_required(VERSION 3.15)

# Enable package manager (Hunter)
include(cmake/PackageManager.cmake)

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

# include(cmake/Packages.cmake)
hunter_add_package(GTest)

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

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
             "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
             LIBRARY_OUTPUT_DIRECTORY
             "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
             RUNTIME_OUTPUT_DIRECTORY
             "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
  set_target_properties(
    ${PROJECT_NAME}_LIB
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
               "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
               LIBRARY_OUTPUT_DIRECTORY
               "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
               OUTPUT_NAME ${PROJECT_NAME})
endif()

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

# Install library for easy downstream inclusion
include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION include
  PUBLIC_HEADER DESTINATION include)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME_LOWERCASE}/version.hpp
  DESTINATION include/${PROJECT_NAME_LOWERCASE})

# Install the `include` directory
install(DIRECTORY include/${PROJECT_NAME_LOWERCASE} DESTINATION include)

message(
  STATUS
    "Install targets succesfully build. Install with `cmake --build <build_directory> --target install --config <build_config>`."
)

message(STATUS "Finished building requirements for installing the package.\n")

# Unit testing setup
if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
  enable_testing()
  message(
    STATUS
      "Build unit tests for the project."
  )
  add_subdirectory(test)
endif()
