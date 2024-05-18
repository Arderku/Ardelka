cmake_minimum_required(VERSION 3.27)

project(Engine)

# Specify the GLSL compiler
find_program(GLSLC_EXECUTABLE glslc HINTS "${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}")

if(NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc compiler not found. Please ensure the Vulkan SDK is installed and glslc is in your PATH.")
endif()

# Define the shaders and the corresponding SPIR-V output files
set(SHADER_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/vert.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/frag.glsl"
)

set(SHADER_OUTPUTS
        "${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/vert.spv"
        "${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/frag.spv"
)

# Add custom commands to compile shaders with -fshader-stage option
add_custom_command(
        OUTPUT ${SHADER_OUTPUTS}
        COMMAND ${GLSLC_EXECUTABLE} -fshader-stage=vertex -o ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/vert.spv ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/vert.glsl
        COMMAND ${GLSLC_EXECUTABLE} -fshader-stage=fragment -o ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/frag.spv ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/frag.glsl
        DEPENDS ${SHADER_SOURCES}
        COMMENT "Compiling shaders..."
)

# Add a custom target to build shaders
add_custom_target(Shaders ALL DEPENDS ${SHADER_OUTPUTS})

# Gather all source and header files
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp" "src/*.h")

# Define the library
add_library(Engine STATIC ${ENGINE_SOURCES}
        src/Graphics/CommandBufferManager.cpp)

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
target_link_libraries(Engine PUBLIC Vulkan::Vulkan glfw spdlog::spdlog)

# Ensure Engine depends on Shaders target
add_dependencies(Engine Shaders)
