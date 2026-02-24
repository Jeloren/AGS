#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

Shader::Shader() : program(0) {}

Shader::~Shader() {
    if (program != 0) glDeleteProgram(program);
}

void Shader::activate() {
    glUseProgram(program);
}

void Shader::deactivate() {
    glUseProgram(0);
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

    // Проверка ошибок компиляции
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

    // Проверка линковки
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

    // Шейдеры можно удалить после линковки
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return true;
}