#pragma once
#include <glm/glm.hpp>

class Material {
public:
    Material();
    Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess);

    void setAmbient(const glm::vec3& ambient);
    void setDiffuse(const glm::vec3& diffuse);
    void setSpecular(const glm::vec3& specular);
    void setShininess(float shininess);

    const glm::vec3& getAmbient() const;
    const glm::vec3& getDiffuse() const;
    const glm::vec3& getSpecular() const;
    float getShininess() const;

private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};