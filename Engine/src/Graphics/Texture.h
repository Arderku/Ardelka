#pragma once

#include <string>

class Texture {
public:
    virtual ~Texture() = default;
    virtual void Load(const std::string& filePath) = 0;
    virtual void Bind() const = 0;
};

