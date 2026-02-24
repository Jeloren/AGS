#pragma once
#include <string>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    Shader();
    ~Shader();
    
    // Загрузка шейдеров и линковка программы
    bool load(std::string vertexShaderFilename, std::string fragmentShaderFilename);
    
    // Активация шейдера (с оптимизацией)
    void activate();
    
    // Деактивация (с оптимизацией)
    static void deactivate();
    
    GLuint getProgramID() const { return program; }

    // Установка uniform переменных
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::vec2& vector);
    void setUniform(const std::string& name, const glm::vec4& vector);
    void setUniform(const std::string& name, const glm::mat4& matrix);

private:
    GLuint program;
    
    // Статическое поле для отслеживания активной программы
    static GLuint currentProgram;

    // Контейнер для кэширования локаций uniform-переменных
    // Используем GLint, так как glGetUniformLocation может вернуть -1
    std::map<std::string, GLint> uniforms;

    // Получение локации с кэшированием
    GLint getUniformLocation(const std::string& name);

    // Вспомогательная функция чтения файла
    std::string readFile(const std::string& filename);
    
    // Компиляция отдельного шейдера
    GLuint createShaderObject(GLenum shaderType, std::string sourceCode);
};