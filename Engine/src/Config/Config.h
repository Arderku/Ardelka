#pragma once
#include <string>

struct WindowSettings {
    std::string title;
    int width;
    int height;
    bool fullscreen;
};

struct EngineSettings {
    WindowSettings windowSettings;
};