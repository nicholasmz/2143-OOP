cmake_minimum_required(VERSION 3.10)
project(PatternRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})

# Add the executable
add_executable(program2 main.cpp)

# Link SDL2
target_link_libraries(program2 ${SDL2_LIBRARIES})

# Copy shapes.json to build directory
configure_file(shapes.json shapes.json COPYONLY)
