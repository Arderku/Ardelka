#include <iostream>
#include "Ardelka.h"  // Assuming this includes all necessary headers from the Engine

int main() {

    Ardelka::Logger::Init();
    AR_CORE_INFO("Logger initialized");
    std::cout << "Sandbox starting...\n";


    Window window("Sandbox", 1280, 640);
    window.SetEventCallback([](GLFWwindow *window, int width, int height) {

    });

    Renderer renderer(window);
    renderer.Initialize();

    while (!window.ShouldClose()) {
        window.OnUpdate();
    }

    return 0;
}
