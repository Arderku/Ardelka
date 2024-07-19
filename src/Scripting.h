#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include "Scene.h"

class Scripting {
public:
    void Init();
    void ExecuteScripts(Scene& scene);
    void Shutdown();
private:
    MonoDomain* domain;
    MonoAssembly* assembly;
    MonoImage* image;
};
