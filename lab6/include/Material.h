#pragma once
#include <glm/glm.hpp>
#include <string>
#include <GL/glew.h>

using namespace glm;

// Класс для представления материала объекта
class Material
{
public:
    // Конструктор по умолчанию
    Material();
    
    // Загрузка материала из JSON файла
    bool loadFromJson(const std::string& filename);
    
    // Геттеры
    glm::vec4 getAmbient() const;
    glm::vec4 getDiffuse() const;
    glm::vec4 getSpecular() const;
    float getShininess() const;

private:
    glm::vec4 ambient;    // Фоновая компонента
    glm::vec4 diffuse;    // Диффузная компонента
    glm::vec4 specular;   // Зеркальная компонента
    GLfloat shininess;    // Степень отполированности
};
