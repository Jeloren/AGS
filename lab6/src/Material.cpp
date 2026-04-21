#include "Material.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <iostream>

using namespace rapidjson;

Material::Material() :
    ambient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)),
    diffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)),
    specular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)),
    shininess(32.0f)
{
}

bool Material::loadFromJson(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open material file: " << filename << std::endl;
        return false;
    }

    IStreamWrapper isw(file);
    Document doc;
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        std::cerr << "Error parsing JSON: " << filename << std::endl;
        return false;
    }

    // Загрузка ambient
    if (doc.HasMember("ambient") && doc["ambient"].IsArray()) {
        const auto& ambArray = doc["ambient"].GetArray();
        if (ambArray.Size() >= 4) {
            ambient = glm::vec4(
                ambArray[0].GetFloat(),
                ambArray[1].GetFloat(),
                ambArray[2].GetFloat(),
                ambArray[3].GetFloat()
            );
        }
    }

    // Загрузка diffuse
    if (doc.HasMember("diffuse") && doc["diffuse"].IsArray()) {
        const auto& diffArray = doc["diffuse"].GetArray();
        if (diffArray.Size() >= 4) {
            diffuse = glm::vec4(
                diffArray[0].GetFloat(),
                diffArray[1].GetFloat(),
                diffArray[2].GetFloat(),
                diffArray[3].GetFloat()
            );
        }
    }

    // Загрузка specular
    if (doc.HasMember("specular") && doc["specular"].IsArray()) {
        const auto& specArray = doc["specular"].GetArray();
        if (specArray.Size() >= 4) {
            specular = glm::vec4(
                specArray[0].GetFloat(),
                specArray[1].GetFloat(),
                specArray[2].GetFloat(),
                specArray[3].GetFloat()
            );
        }
    }

    // Загрузка shininess
    if (doc.HasMember("shininess") && doc["shininess"].IsNumber()) {
        shininess = doc["shininess"].GetFloat();
    }

    return true;
}

glm::vec4 Material::getAmbient() const
{
    return ambient;
}

glm::vec4 Material::getDiffuse() const
{
    return diffuse;
}

glm::vec4 Material::getSpecular() const
{
    return specular;
}

float Material::getShininess() const
{
    return shininess;
}
