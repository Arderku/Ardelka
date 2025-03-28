#include <iostream>
#include "Window.h"

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->m_Width = width;
        win->m_Height = height;

        if (win->m_ResizeCallback) {
            win->m_ResizeCallback(width, height);
        }
    }
}

void Window::Create(int width, int height, const char* title) {
    if (!glfwInit()) return;

    // Set OpenGL version to 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Width = width;
    m_Height = height;

    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);

    // Set the framebuffer size callback
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
    glfwSetWindowUserPointer(m_Window, this);

    // Enable V-Sync
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return;
    }

    // Query the OpenGL version
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "OpenGL Version: " << version << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;
    std::cout << "GLSL Version: " << glslVersion << std::endl;

    std::cout << "Window created successfully." << std::endl;

    // Set the initial viewport size
    glViewport(0, 0, width, height);
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

bool Window::IsOpen() const {
    return !glfwWindowShouldClose(m_Window);
}

void Window::SetResizeCallback(ResizeCallback callback) {
    m_ResizeCallback = callback;
}
