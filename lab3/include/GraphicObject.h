#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GraphicObject {
public:
    GraphicObject(glm::vec3 pos, float angle, glm::vec4 col);

    void recalculateModelMatrix();
    
    glm::mat4 getModelMatrix() const;
    glm::vec4 getColor() const;

    // Свойства сделаны публичными для простоты изменения извне (анимации)
    glm::vec3 position;
    float angleY;
    glm::vec4 color;
    
    void setScale(const glm::vec3& s) { 
        scale = s; 
        recalculateModelMatrix(); // Обязательно пересчитываем матрицу при смене размера
    }

private:
    glm::mat4 modelMatrix;
    glm::vec3 scale = glm::vec3(1.0f); // По умолчанию размер 1.0
};