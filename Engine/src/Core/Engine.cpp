#include <Ardelkapch.h>
#include <thread>
#include "Engine.h"

Engine::Engine() : isRunning(false) {}

Engine::~Engine() {
    Shutdown();
}

void Engine::Initialize() {
    // Initialize all subsystems
    isRunning = true;
}

void Engine::Run() {
    int testNumber = 0;
    while (isRunning) {
        std::cout << "\rEngine Run: " << testNumber++ << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Slow down the increment for visibility
    }
}

void Engine::Shutdown() {
    // Clean up resources
    isRunning = false;
}