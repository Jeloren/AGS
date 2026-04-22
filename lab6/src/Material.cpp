#include "Material.h"

Material::Material()
    : ambient(0.1f, 0.1f, 0.1f), diffuse(0.8f, 0.8f, 0.8f), specular(0.0f, 0.0f, 0.0f), shininess(1.0f) {}

Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
    : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

void Material::setAmbient(const glm::vec3& ambient) {
    this->ambient = ambient;
}

void Material::setDiffuse(const glm::vec3& diffuse) {
    this->diffuse = diffuse;
}

void Material::setSpecular(const glm::vec3& specular) {
    this->specular = specular;
}

void Material::setShininess(float shininess) {
    this->shininess = shininess;
}

const glm::vec3& Material::getAmbient() const {
    return ambient;
}

const glm::vec3& Material::getDiffuse() const {
    return diffuse;
}

const glm::vec3& Material::getSpecular() const {
    return specular;
}

float Material::getShininess() const {
    return shininess;
}