cmake_minimum_required(VERSION 3.10)

# Editor module
project(Editor)

# Editor source files
file(GLOB EDITOR_SOURCES "src/*.cpp")

# Define the executable
add_executable(Editor ${EDITOR_SOURCES} )

# Include directories from Engine and ImGui
target_include_directories(Editor PRIVATE
        ../Engine/include
        ../Engine/src  # Include this if there are internal headers necessary
        ${PROJECT_SOURCE_DIR}/../ThirdParty/ImGui
        ${PROJECT_SOURCE_DIR}/../ThirdParty/ImGui/backends
)

# Link with the Engine library and ImGui
target_link_libraries(Editor Engine ImGui)
