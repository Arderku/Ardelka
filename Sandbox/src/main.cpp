#include <iostream>
#include "Ardelka.h"  // Assuming this includes all necessary headers from the Engine

int main() {
    std::cout << "Editor starting...\n";

    // Initialize the engine from the Editor context
    Engine engine;
    engine.Initialize();
    std::cout << "Engine initialized.\n";

    // Simulation of editor tasks or run loop
    engine.Run();

    // Clean up
    engine.Shutdown();
    std::cout << "Editor shutting down.\n";

    Window window("Sandbox", 1280, 640);
    window.SetEventCallback([](GLFWwindow *window, int width, int height) {

    });

    while (!window.ShouldClose()) {
        std::cout << "Window running...\n";
        window.OnUpdate();
    }


    return 0;
}
