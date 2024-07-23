#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Light.h"

class Shader {
public:
    // Constructor: Loads, compiles, and links vertex and fragment shaders
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    ~Shader();
    // Use/Activate the shader program
    void use() const;

    // Deactivate the shader program
    void unuse() const;

    // Utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    // Setting light uniforms
    void setDirectionalLight(const std::string &name, const DirectionalLight &light) const;
    void setPointLight(const std::string &name, const PointLight &light) const;

    // Check if the shader program is valid
    bool IsValid() const;

    // Get the program ID
    unsigned int getProgramID() const;

private:
    unsigned int programID;

    // Read shader code from file
    std::string readShaderCode(const std::string& filePath) const;

    // Compile a shader from source code
    GLuint compileShader(const std::string& source, GLenum shaderType) const;

    // Check for compilation and linking errors
    void checkCompileErrors(GLuint shader, const std::string& type) const;
};
