cmake_minimum_required(VERSION 3.10)

# Engine module
project(Engine)

# Gather all source and header files
file(GLOB_RECURSE ENGINE_SOURCES
        "src/*.cpp"
        "src/*.h"
)

# Define the library
add_library(Engine STATIC ${ENGINE_SOURCES}
        src/Graphics/Renderer.cpp
        src/Graphics/Renderer.h
        src/Graphics/Renderer.h
        src/Graphics/VulkanContext.cpp
        src/Graphics/VulkanContext.h
        src/Graphics/VulkanContext.h)

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
