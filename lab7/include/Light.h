#pragma once
#include <glm/glm.hpp>

using namespace glm;

// Класс для представления направленного источника света
class Light
{
public:
    // Конструктор по умолчанию
    Light();
    
    // Сеттеры
    void setDirection(const glm::vec3& direction);
    void setAmbient(const glm::vec4& ambient);
    void setDiffuse(const glm::vec4& diffuse);
    void setSpecular(const glm::vec4& specular);
    
    // Геттеры
    glm::vec4 getDirection() const;
    glm::vec4 getAmbient() const;
    glm::vec4 getDiffuse() const;
    glm::vec4 getSpecular() const;

private:
    glm::vec4 direction;  // Направление источника света
    glm::vec4 ambient;    // Фоновая составляющая
    glm::vec4 diffuse;    // Диффузная составляющая
    glm::vec4 specular;   // Зеркальная составляющая
};
