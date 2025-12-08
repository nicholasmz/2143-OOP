cmake_minimum_required(VERSION 3.10)
project(GameOfLifeSDL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SDL2 path
set(SDL2_DIR "C:/SDL2/SDL2-2.28.5")
set(SDL2_INCLUDE_DIRS "${SDL2_DIR}/include")
set(SDL2_LIBRARIES "${SDL2_DIR}/lib/x64/SDL2.lib;${SDL2_DIR}/lib/x64/SDL2main.lib")

# Find SDL2
find_package(SDL2 QUIET)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Source files
set(SOURCES
    src/SDL_GOL_main.cpp
    src/ConwayLife.cpp
    src/SdlScreen.cpp
)

# Create executable
add_executable(SDL_GOL_main ${SOURCES})

# Link SDL2
target_link_libraries(SDL_GOL_main ${SDL2_LIBRARIES})

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
