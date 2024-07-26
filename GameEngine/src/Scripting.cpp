#include "Scripting.h"
#include <iostream>

void Scripting::Init() {
    mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");
    domain = mono_jit_init("ScriptingDomain");

    assembly = mono_domain_assembly_open(domain, "Scripts.dll");
    if (!assembly) {
        std::cerr << "Failed to load assembly" << std::endl;
        return;
    }

    image = mono_assembly_get_image(assembly);
}

void Scripting::ExecuteScripts(Scene& scene) {
    MonoClass* klass = mono_class_from_name(image, "GameNamespace", "GameScript");
    if (!klass) {
        std::cerr << "Failed to find class" << std::endl;
        return;
    }

    MonoObject* obj = mono_object_new(domain, klass);
    mono_runtime_object_init(obj);

    MonoMethod* method = mono_class_get_method_from_name(klass, "Update", 1);
    if (!method) {
        std::cerr << "Failed to find method" << std::endl;
        return;
    }

    void* args[1];
    args[0] = &scene;
    mono_runtime_invoke(method, obj, args, nullptr);
}

void Scripting::Shutdown() {
    mono_jit_cleanup(domain);
}
