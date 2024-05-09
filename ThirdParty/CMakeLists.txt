# Optional: Build GLFW from source if it's part of the project
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(GLFW)

# Add ImGui library
file(GLOB IMGUI_SOURCES
        "${PROJECT_SOURCE_DIR}/ThirdParty/ImGui/*.cpp"
        "${PROJECT_SOURCE_DIR}/ThirdParty/ImGui/backends/imgui_impl_vulkan.cpp"
        "${PROJECT_SOURCE_DIR}/ThirdParty/ImGui/backends/imgui_impl_glfw.cpp"
)
add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC
        "${PROJECT_SOURCE_DIR}/ThirdParty/ImGui"
        "${PROJECT_SOURCE_DIR}/ThirdParty/ImGui/backends"
)

# Find Vulkan package
find_package(Vulkan REQUIRED)
target_link_libraries(ImGui PUBLIC Vulkan::Vulkan glfw)
