#pragma once

class Shader;

class Pipeline {
public:
    virtual ~Pipeline() = default;
    virtual void Create() = 0;
    virtual void Bind() const = 0;
};
