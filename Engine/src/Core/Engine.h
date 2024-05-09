//
// Created by arder on 5/10/2024.
//

#ifndef ARDELKA_ENGINE_H
#define ARDELKA_ENGINE_H


#pragma once

class Engine {
public:
    Engine();
    ~Engine();

    void Initialize();
    void Run();
    void Shutdown();

private:
    bool isRunning;
};


#endif //ARDELKA_ENGINE_H
