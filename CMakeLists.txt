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

# Fetch and build googletest
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
FetchContent_MakeAvailable(ftxui)
 

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Set the directory for storing all executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# For multi-config generators
if(CMAKE_CONFIGURATION_TYPES)
  foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_SOURCE_DIR}/bin)
  endforeach()
endif()

# Define Debug and Release build types
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
# Enable warnings globally
add_compile_options(-Wall -Wextra -Wpedantic)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/inc ${PROJECT_SOURCE_DIR}/rapid_xml ${PROJECT_SOURCE_DIR}/oof)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cxx src/*.cpp)

add_executable(dcu ${PROJECT_SOURCE_DIR}/tools/dcu.cxx ${PROJECT_SOURCES})
target_link_libraries(dcu
  PRIVATE dim
  PRIVATE pthread
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)