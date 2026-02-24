#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Инициализация статического поля
GLuint Shader::currentProgram = 0;

Shader::Shader() : program(0) {}

Shader::~Shader() {
    if (program != 0) glDeleteProgram(program);
}

void Shader::activate() {
    // Оптимизация: меняем контекст только если программа отличается
    if (currentProgram != program) {
        glUseProgram(program);
        currentProgram = program;
    }
}

void Shader::deactivate() {
    if (currentProgram != 0) {
        glUseProgram(0);
        currentProgram = 0;
    }
}

GLint Shader::getUniformLocation(const std::string& name) {
    // Ищем переменную в кэше
    auto it = uniforms.find(name);
    if (it != uniforms.end()) {
        return it->second;
    }
    // Если не найдена, запрашиваем у OpenGL
    GLint location = glGetUniformLocation(program, name.c_str());
    uniforms[name] = location; // Сохраняем в кэш
    return location;
}

void Shader::setUniform(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, const glm::vec2& vector) {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, const glm::vec4& vector) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::createShaderObject(GLenum shaderType, std::string sourceCode) {
    GLuint shaderID = glCreateShader(shaderType);
    const char* srcPtr = sourceCode.c_str();
    glShaderSource(shaderID, 1, &srcPtr, NULL);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(shaderID, logLength, NULL, log.data());
        std::cerr << "Shader compile error: " << log.data() << std::endl;
        return 0;
    }
    return shaderID;
}

bool Shader::load(std::string vertexFile, std::string fragmentFile) {
    std::string vCode = readFile(vertexFile);
    std::string fCode = readFile(fragmentFile);

    if (vCode.empty() || fCode.empty()) return false;

    GLuint vShader = createShaderObject(GL_VERTEX_SHADER, vCode);
    GLuint fShader = createShaderObject(GL_FRAGMENT_SHADER, fCode);

    if (vShader == 0 || fShader == 0) return false;

    program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetProgramInfoLog(program, logLength, NULL, log.data());
        std::cerr << "Program link error: " << log.data() << std::endl;
        return false;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return true;
}