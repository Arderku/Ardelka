#include <iostream>
#include "Ardelka.h"  // Assuming this includes all necessary headers from the Engine
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

int main() {
    Ardelka::Logger::Init();
    AR_INFO("Logger initialized");
    std::cout << "Sandbox starting...\n";

    Window window("Sandbox", 1280, 640);
    window.SetEventCallback([](GLFWwindow *window, int width, int height) {
        // Handle window resize if needed
    });

    Renderer renderer(window);
    renderer.Initialize();

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForVulkan(window.GetNativeWindow(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};



    while (!window.ShouldClose()) {
        window.OnUpdate();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a simple ImGui window
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        ImGui::Render();

        renderer.Render();
    }

    // Cleanup ImGui
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    renderer.Shutdown();

    return 0;
}
