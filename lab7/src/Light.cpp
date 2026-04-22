#include "Light.h"

Light::Light() :
    direction(glm::vec4(0.0f, -1.0f, -1.0f, 0.0f)),  // Направление вниз и назад
    ambient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)),
    diffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
    specular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
{
}

void Light::setDirection(const glm::vec3& dir)
{
    direction = glm::vec4(glm::normalize(dir), 0.0f);
}

void Light::setAmbient(const glm::vec4& amb)
{
    ambient = amb;
}

void Light::setDiffuse(const glm::vec4& diff)
{
    diffuse = diff;
}

void Light::setSpecular(const glm::vec4& spec)
{
    specular = spec;
}

glm::vec4 Light::getDirection() const
{
    return direction;
}

glm::vec4 Light::getAmbient() const
{
    return ambient;
}

glm::vec4 Light::getDiffuse() const
{
    return diffuse;
}

glm::vec4 Light::getSpecular() const
{
    return specular;
}
