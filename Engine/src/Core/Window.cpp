#include "Window.h"

#include <utility>

Window::Window(std::string  title, int width, int height)
        : m_Title(std::move(title)), m_Width(width), m_Height(height) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);

    // TODO: VSync is enabled by default; this should later have configurable option
    glfwSwapInterval(1);
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::onUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

void Window::setEventCallback(const EventCallbackFn& callback) {
    m_EventCallback = callback;  // Updated to use m_ prefix
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win && win->m_EventCallback) {  // Check for win not being nullptr and use m_ prefix
        win->m_EventCallback(window, width, height);
    }
}
