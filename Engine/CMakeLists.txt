cmake_minimum_required(VERSION 3.10)

# Engine module
project(Engine)

# Gather all source and header files
file(GLOB_RECURSE ENGINE_SOURCES
        "src/*.cpp"
        "src/*.h"
)

# Define the library
add_library(Engine STATIC ${ENGINE_SOURCES})

# Specify include directories
target_include_directories(Engine PUBLIC
        include
        src  # If you use any local headers directly in src
)

# Setup precompiled headers
target_precompile_headers(Engine PUBLIC src/Ardelkapch.h)

# Find Vulkan and link it
find_package(Vulkan REQUIRED)
target_link_libraries(Engine PUBLIC Vulkan::Vulkan)
