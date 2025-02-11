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
include_directories(GameEngine/include)
include_directories(vendor/glfw/include)
include_directories(vendor/imgui)
include_directories(vendor/imgui/backends)
include_directories(vendor/glm)
include_directories(vendor/stb)  # Add this line to include stb_image.h


# Find Assimp package
if (WIN32)
    set(ASSIMP_ROOT "C:/Program Files/Assimp")  # Update this to your Assimp installation path
    set(ASSIMP_INCLUDE_DIR "${ASSIMP_ROOT}/include")
    set(ASSIMP_LIBRARY_DIR "${ASSIMP_ROOT}/lib/x64")
    find_library(ASSIMP_LIB NAMES assimp-vc143-mt PATHS ${ASSIMP_LIBRARY_DIR} NO_DEFAULT_PATH)
    set(ASSIMP_DLL "${ASSIMP_ROOT}/bin/x64/assimp-vc143-mt.dll")
    if (NOT ASSIMP_LIB)
        message(FATAL_ERROR "Assimp library not found. Please check your Assimp installation path. ${ASSIMP_LIBRARY_DIR}")
    endif()
    include_directories(${ASSIMP_INCLUDE_DIR})
else()
    find_package(assimp CONFIG REQUIRED)
endif()

# Add executable
add_executable(GameEngine
        GameEngine/src/main.cpp
        GameEngine/src/Application.cpp
        GameEngine/src/Window.cpp
        GameEngine/src/Renderer.cpp
        GameEngine/src/Scene.cpp
        GameEngine/src/Editor.cpp
        GameEngine/src/Shader.h
        GameEngine/src/Shader.cpp
        GameEngine/src/Material.h
        GameEngine/src/Material.cpp
        GameEngine/src/Component.h
        GameEngine/src/Mesh.h
        GameEngine/src/Mesh.cpp
        GameEngine/src/GameObject.cpp
        GameEngine/src/Light.h
        GameEngine/src/Texture.cpp
        GameEngine/src/Texture.h
        GameEngine/src/ResourceManager.h
        GameEngine/src/ResourceManager.cpp
        GameEngine/src/Camera.h
        GameEngine/src/EditorStyle.h
        GameEngine/src/EditorStyle.cpp
        GameEngine/src/TestObject.cpp
        GameEngine/src/TestObject.h
        GameEngine/src/FBXLoader.cpp
        GameEngine/src/LightComponent.h
        GameEngine/src/Layer.h
        GameEngine/src/EditorLayer.h
        GameEngine/src/GameLayer.h
)

# Link libraries
if (WIN32)
    target_link_libraries(GameEngine
            glfw
            ${GLEW_LIBRARY}
            glm
            imgui
            opengl32
            ${CMAKE_DL_LIBS}
            ${ASSIMP_LIB}
    )
else()
    target_link_libraries(GameEngine
            glfw
            ${GLEW_LIBRARY}
            glm
            imgui
            opengl32
            ${CMAKE_DL_LIBS}
            assimp
    )
endif()

# Copy shader files to the build directory
add_custom_command(
        TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources/
        ${CMAKE_BINARY_DIR}/Resources/
)

# Copy GLEW DLL to the build directory
add_custom_command(
        TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${GLEW_DLL} ${CMAKE_BINARY_DIR}
)

# Copy Assimp DLL to the build directory (only for Windows)
if (WIN32)
    add_custom_command(
            TARGET GameEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ASSIMP_DLL} ${CMAKE_BINARY_DIR}
    )
endif()
