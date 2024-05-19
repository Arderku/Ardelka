#pragma once

#include <string>

class Shader {
public:
    virtual ~Shader() = default;
    virtual void Load(const std::string& vertexPath, const std::string& fragmentPath) = 0;
    virtual void Bind() const = 0;
};
