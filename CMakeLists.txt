cmake_minimum_required(VERSION 3.10)

# Set the project name
project(dim-cmd-line-tool VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Define Debug and Release build types
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
# Enable warnings globally
add_compile_options(-Wall -Wextra -Wpedantic)

# Fetch and build googletest
FetchContent_MakeAvailable(googletest)
# Include directories
include_directories(${PROJECT_SOURCE_DIR}/inc ${PROJECT_SOURCE_DIR}/rapid_xml)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cxx src/*.cpp)
# Enable testing
enable_testing()

# Local test
add_executable(local-test tests/local/main.cxx ${PROJECT_SOURCES})
target_link_libraries(local-test dim pthread)
target_include_directories(local-test PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_include_directories(local-test PRIVATE ${PROJECT_SOURCE_DIR}/oof)
target_include_directories(local-test PRIVATE ${PROJECT_SOURCE_DIR}/rapid_xml)
# Add test executable
file(GLOB_RECURSE TEST_SOURCES tests/*.cxx tests/*.cpp)
#add_executable(run_google_tests ${TEST_SOURCES} ${PROJECT_SOURCES})

# Link test executable against gtest and gtest_main
#target_link_libraries(run_google_tests gtest gtest_main)
#target_include_directories(run_google_tests PRIVATE ${PROJECT_SOURCE_DIR}/inc)
#target_include_directories(run_google_tests PRIVATE ${PROJECT_SOURCE_DIR}/oof)

# Add tests
#add_test(NAME GOOGLE_TESTS COMMAND run_google_tests)