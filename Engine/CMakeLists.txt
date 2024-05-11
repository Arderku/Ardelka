cmake_minimum_required(VERSION 3.27)  # Updated version

project(Engine)

# Gather all source and header files
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp" "src/*.h")

# Define the library
add_library(Engine STATIC ${ENGINE_SOURCES}
        src/Config/Config.h
        src/Config/ConfigurationManager.h)

# Specify include directories
target_include_directories(Engine PUBLIC
        include
        src
        ${PROJECT_SOURCE_DIR}/ThirdParty/GLFW/include
)

# Setup precompiled headers
target_precompile_headers(Engine PUBLIC src/Ardelkapch.h)

# Find Vulkan and link it
find_package(Vulkan REQUIRED)
target_link_libraries(Engine PUBLIC Vulkan::Vulkan glfw)
