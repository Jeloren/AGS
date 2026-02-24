#include "GraphicObject.h"

GraphicObject::GraphicObject(glm::vec3 pos, float angle, glm::vec4 col) 
    : position(pos), angleY(angle), color(col) {
    recalculateModelMatrix();
}

void GraphicObject::recalculateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 GraphicObject::getModelMatrix() const {
    return modelMatrix;
}

glm::vec4 GraphicObject::getColor() const {
    return color;
}