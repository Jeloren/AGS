#pragma once
#include <string>
#include <GL/glew.h>

class Shader {
public:
    Shader();
    ~Shader();
    // Загрузка шейдеров и линковка программы
    bool load(std::string vertexShaderFilename, std::string fragmentShaderFilename);
    // Активация шейдера
    void activate();
    // Деактивация
    static void deactivate();
    // Получить ID программы (может пригодиться для uniform переменных позже)
    GLuint getProgramID() const { return program; }

private:
    GLuint program;
    // Вспомогательная функция чтения файла
    std::string readFile(const std::string& filename);
    // Компиляция отдельного шейдера
    GLuint createShaderObject(GLenum shaderType, std::string sourceCode);
};