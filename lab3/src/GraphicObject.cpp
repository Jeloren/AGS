#include "GraphicObject.h"

GraphicObject::GraphicObject(glm::vec3 pos, float angle, glm::vec4 col, glm::vec3 scale) 
    : position(pos), angleY(angle), color(col), scale(scale) {
    recalculateModelMatrix();
}

void GraphicObject::recalculateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
}

glm::mat4 GraphicObject::getModelMatrix() const {
    return modelMatrix;
}

glm::vec4 GraphicObject::getColor() const {
    return color;
}