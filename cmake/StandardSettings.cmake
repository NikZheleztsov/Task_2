# Project settings
option(${PROJECT_NAME}_BUILD_EXECUTABLE "Build the project as an executable, rather than a library." ON)
option(${PROJECT_NAME}_VERBOSE_OUTPUT "Enable verbose output, allowing for a better understanding of each step taken." ON)

# Compiler options
option(${PROJECT_NAME}_WARNINGS_AS_ERRORS "Treat compiler warnings as errors." ON)

# Unit testing
option(${PROJECT_NAME}_ENABLE_UNIT_TESTING "Enable unit tests for the projects (from the `test` subfolder)." ON)
option(${PROJECT_NAME}_USE_CATCH2 "Use Catch2 for creating unit tests." OFF)
option(${CMAKE_PROJECT_NAME}_USE_GTEST OFF)

# Static analyzer
option(${PROJECT_NAME}_ENABLE_CPPCHECK "Enable static analysis with Cppcheck." ON)

# Code coverage
option(${PROJECT_NAME}_ENABLE_CODE_COVERAGE "Enable code coverage through GCC." ON)
