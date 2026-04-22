#include "GraphicObject.h"

GraphicObject::GraphicObject() :
	meshId(0),
	textureId(0),
	materialId(-1),
	color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
	modelMatrix(glm::mat4(1.0f)),
	position(glm::vec3(0.0f)),
	angleY(0.0f),
	angleX(0.0f),
	angleZ(0.0f),
	scale(glm::vec3(1.0f))
	{}

void GraphicObject::updateModelMatrix() {
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);
}

void GraphicObject::setColor(const vec4& color) {
	this->color = color;
}

void GraphicObject::setPosition(const vec3& position) {
	this->position = position;
	updateModelMatrix();
}

void GraphicObject::setAngle(float degree) {
	this->angleY = degree;
	updateModelMatrix();
}

void GraphicObject::setRotation(float angleX, float angleY, float angleZ) {
	this->angleX = angleX;
	this->angleY = angleY;
	this->angleZ = angleZ;
	updateModelMatrix();
}

void GraphicObject::setScale(float scale) {
	setScale(scale, scale, scale);
}

void GraphicObject::setScale(float scaleX, float scaleY, float scaleZ) {
	this->scale = glm::vec3(scaleX, scaleY, scaleZ);
	updateModelMatrix();
}

void GraphicObject::setMeshId(int id) {
	this->meshId = id;
}

vec4& GraphicObject::getColor() {
	return color;
}

mat4& GraphicObject::getModelMatrix() {
	return modelMatrix;
}

int GraphicObject::getMeshId() const {
	return meshId;
}

void GraphicObject::setTextureId(int id) {
	this->textureId = id;
}

int GraphicObject::getTextureId() const {
	return textureId;
}

const vec4& GraphicObject::getColor() const {
	return color;
}

const mat4& GraphicObject::getModelMatrix() const {
	return modelMatrix;
}

void GraphicObject::setMaterialId(int id) {
	this->materialId = id;
}

int GraphicObject::getMaterialId() const {
	return materialId;
}vec3 GraphicObject::getPosition() const {
return position;
}
