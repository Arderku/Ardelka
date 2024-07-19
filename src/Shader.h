#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Light.h"

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    void setDirectionalLight(const std::string &name, const DirectionalLight &light) const;
    void setPointLight(const std::string &name, const PointLight &light) const;

    bool IsValid() const;

    unsigned int getProgramID() const;

    void unuse() const;

private:
    unsigned int programID;
    void checkCompileErrors(GLuint shader, const std::string& type) const;

};
