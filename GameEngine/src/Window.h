#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <functional>

class Window {
public:
    using ResizeCallback = std::function<void(int, int)>;

    void Create(int width, int height, const char* title);
    void PollEvents();
    void SwapBuffers();
    bool IsOpen() const;
    GLFWwindow* GetWindow() const { return m_Window; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    void SetResizeCallback(ResizeCallback callback);

private:
    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
    ResizeCallback m_ResizeCallback;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};
