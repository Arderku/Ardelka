#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Light.h"

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;
    void unuse() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    void setDirectionalLight(const std::string &name, const DirectionalLight &light) const;
    void setPointLight(const std::string &name, const PointLight &light) const;

    bool IsValid() const;

private:
    unsigned int programID;
    std::string readShaderCode(const std::string& filePath) const;
    std::string preprocessShaderCode(const std::string& code, const std::string& parentPath) const;
    unsigned int compileShader(const std::string& source, GLenum shaderType) const;
    void checkCompileErrors(unsigned int shader, const std::string& type, const std::string& source) const; // Add source parameter
    unsigned int getProgramID() const;
};
