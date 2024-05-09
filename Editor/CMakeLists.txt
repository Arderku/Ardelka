cmake_minimum_required(VERSION 3.10)

# Editor module
project(Editor)

# Editor source files
file(GLOB EDITOR_SOURCES "src/*.cpp")

# Define the executable
add_executable(Editor ${EDITOR_SOURCES})

# Include directories from Engine
target_include_directories(Editor PRIVATE
        ../Engine/include
        ../Engine/src  # Only if needed for internal headers
)

# Link with the Engine library
target_link_libraries(Editor Engine)
