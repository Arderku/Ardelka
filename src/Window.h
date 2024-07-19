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
private:
    GLFWwindow* m_Window;
};
