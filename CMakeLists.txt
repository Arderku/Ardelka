cmake_minimum_required(VERSION 3.27)
project(Ardelka)

# Set the C++ standard for the entire project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Organize the output binary files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add subdirectories for each component of the project
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Sandbox)
add_subdirectory(ThirdParty)

