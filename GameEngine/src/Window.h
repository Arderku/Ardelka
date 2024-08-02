#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>

class Window {
public:
    void Create(int width, int height, const char* title);
    void PollEvents();
    void SwapBuffers();
    bool IsOpen() const;
    GLFWwindow* GetWindow() const { return m_Window; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
private:
    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
};
