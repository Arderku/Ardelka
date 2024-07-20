cmake_minimum_required(VERSION 3.10)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)

# Add subdirectories for dependencies
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)

# GLEW
set(GLEW_ROOT "${CMAKE_SOURCE_DIR}/vendor/glew")
set(GLEW_INCLUDE_DIR "${GLEW_ROOT}/include")
set(GLEW_LIBRARY "${GLEW_ROOT}/lib/Release/x64/glew32s.lib")
set(GLEW_DLL "${GLEW_ROOT}/bin/Release/x64/glew32.dll")

include_directories(${GLEW_INCLUDE_DIR})

# Include directories
include_directories(include)
include_directories(vendor/glfw/include)
include_directories(vendor/imgui)
include_directories(vendor/imgui/backends)
include_directories(vendor/glm)
include_directories(vendor/stb)  # Add this line to include stb_image.h

# Find Mono installation
if (WIN32)
    set(MONO_ROOT "C:/Program Files/Mono")
    include_directories("${MONO_ROOT}/include/mono-2.0")
    link_directories("${MONO_ROOT}/lib")
    set(MONO_LIBRARIES mono-2.0-sgen)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(MONO REQUIRED mono-2)
    include_directories(${MONO_INCLUDE_DIRS})
    link_directories(${MONO_LIBRARY_DIRS})
    set(MONO_LIBRARIES ${MONO_LIBRARIES})
endif()

# Add executable
add_executable(GameEngine
        src/main.cpp
        src/Engine.cpp
        src/Window.cpp
        src/Renderer.cpp
        src/Scene.cpp
        src/Scripting.cpp
        src/Editor.cpp
        src/Shader.h
        src/Shader.cpp
        src/Material.h
        src/Material.cpp
        src/Component.h
        src/Application.h
        src/Application.cpp
        src/Mesh.h
        src/Mesh.cpp
        src/GameObject.cpp
        src/Light.h
        src/Texture.cpp
        src/Texture.h
        src/ResourceManager.h
        src/ResourceManager.cpp
)

# Link libraries
target_link_libraries(GameEngine
        glfw
        ${GLEW_LIBRARY}
        glm
        imgui
        opengl32
        ${CMAKE_DL_LIBS}
        ${MONO_LIBRARIES}
)

# Copy shader files to the build directory
add_custom_command(
        TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/
        ${CMAKE_BINARY_DIR}/resources/
)

# Copy GLEW DLL to the build directory
add_custom_command(
        TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${GLEW_DLL} ${CMAKE_BINARY_DIR}
)
