cmake_minimum_required(VERSION 3.10)

# Set the project name
project(dim-cmd-line-tool VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cxx src/*.cpp)
# Enable testing
enable_testing()

# Add test executable
file(GLOB_RECURSE TEST_SOURCES tests/*.cxx tests/*.cpp)
add_executable(run_google_tests ${TEST_SOURCES} ${PROJECT_SOURCES})
# Link test executable against gtest and gtest_main
target_link_libraries(run_google_tests gtest gtest_main)
target_include_directories(run_google_tests PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_include_directories(run_google_tests PRIVATE ${PROJECT_SOURCE_DIR}/oof)
# Add tests
add_test(NAME GOOGLE_TESTS COMMAND run_google_tests)