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
