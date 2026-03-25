#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GraphicObject {
public:
    GraphicObject(glm::vec3 pos, float angle, glm::vec4 col, glm::vec3 scale);

    void recalculateModelMatrix();
    
    glm::mat4 getModelMatrix() const;
    glm::vec4 getColor() const;

    // Свойства сделаны публичными для простоты изменения извне (анимации)
    glm::vec3 position;
    float angleY;
    glm::vec4 color;
    glm::vec3 scale;

private:
    glm::mat4 modelMatrix;
};