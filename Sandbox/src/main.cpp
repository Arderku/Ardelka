#include <iostream>
#include "Ardelka.h"  // Assuming this includes all necessary headers from the Engine

int main() {
    std::cout << "Sandbox starting...\n";

    // Initialize the engine from the Editor context
    Engine engine;
    engine.Initialize();
    std::cout << "Engine initialized.\n";

    // Simulation of editor tasks or run loop
    engine.Run();

    Window window("Sandbox", 1280, 640);
    window.SetEventCallback([](GLFWwindow *window, int width, int height) {

    });

    Renderer renderer;
    renderer.Initialize();

    while (!window.ShouldClose()) {
        //std::cout << "Window running...\n";
        window.OnUpdate();
        renderer.Render();
    }

    return 0;
}
