// Window.h
#pragma once

#include <Ardelkapch.h>
#include "GLFW/glfw3.h"

class Window {
public:
    using EventCallbackFn = std::function<void(GLFWwindow*, int, int)>;

    Window(std::string  title, int width, int height);
    ~Window();

    void OnUpdate();

    bool ShouldClose() const;
    void SetEventCallback(const EventCallbackFn& callback);

    GLFWwindow* GetNativeWindow();

private:
    GLFWwindow* m_Window;  // Adjust naming to follow your convention
    int m_Width;
    int m_Height;
    std::string m_Title;
    EventCallbackFn m_EventCallback;  // Apply the same convention to all member variables

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};
