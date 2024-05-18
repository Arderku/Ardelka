cmake_minimum_required(VERSION 3.27)

project(Sandbox)

add_executable(Sandbox
        src/main.cpp
        # Add other source files as necessary
)

target_include_directories(Sandbox PRIVATE
        ../Engine/include
)

target_link_libraries(Sandbox PRIVATE
        Engine
)

# Copy Resources directory to the build directory
# Copy Resources directory to the build directory
add_custom_command(
        TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/Engine/Resources $<TARGET_FILE_DIR:Sandbox>/Resources
)
