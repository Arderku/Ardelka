#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
        : programID(0) {
    std::string vertexCode, fragmentCode;

    try {
        vertexCode = readShaderCode(vertexPath);
        fragmentCode = readShaderCode(fragmentPath);
    } catch (const std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return;
    }

    GLuint vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

    if (vertex == 0 || fragment == 0) {
        return;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    std::cout << "Shader program created successfully. Program ID: " << programID << std::endl;
}

Shader::~Shader() {
    if (programID != 0) {
        glDeleteProgram(programID);
    }
}

void Shader::use() const {
    glUseProgram(programID);
}

void Shader::unuse() const {
    glUseProgram(0);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setDirectionalLight(const std::string &name, const DirectionalLight &light) const {
    setVec3(name + ".direction", light.direction);
    setVec3(name + ".color", light.color);
}

void Shader::setPointLight(const std::string &name, const PointLight &light) const {
    setVec3(name + ".position", light.position);
    setVec3(name + ".color", light.color);
    setFloat(name + ".constant", light.constant);
    setFloat(name + ".linear", light.linear);
    setFloat(name + ".quadratic", light.quadratic);
}

bool Shader::IsValid() const {
    return programID != 0;
}

std::string Shader::readShaderCode(const std::string& filePath) const {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::stringstream shaderStream;
    shaderFile.open(filePath);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}

GLuint Shader::compileShader(const std::string& source, GLenum shaderType) const {
    GLuint shader = glCreateShader(shaderType);
    const char* shaderCode = source.c_str();
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) const {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(programID, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

unsigned int Shader::getProgramID() const {
    return programID;
}
