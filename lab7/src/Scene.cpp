#include "Scene.h"
#include "FrustumCulling.h"
#include <iostream>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "ResourceManager.h"

using namespace std;

Scene::Scene()
{
    // Инициализация максимальных дистанций по умолчанию
    maxDistances[static_cast<int>(ObjectType::SMALL_NATURE)] = 500.0f;
    maxDistances[static_cast<int>(ObjectType::BIG_PROP)] = 400.0f;
    maxDistances[static_cast<int>(ObjectType::MEDIUM_PROP)] = 300.0f;
    maxDistances[static_cast<int>(ObjectType::SMALL_PROP)] = 200.0f;

    // Инициализация счетчиков
    totalObjectCount = 0;
    renderedObjectCount = 0;
}

void Scene::addObject(const GraphicObject& obj, ObjectType type, const vec3& dimensions)
{
    SceneObject sceneObj;
    sceneObj.graphicObject = obj;
    sceneObj.type = type;

    // Создание bounding box на основе dimensions
    sceneObj.boundingBox.dimensions = dimensions;
    sceneObj.boundingBox.min = -dimensions * 0.5f;
    sceneObj.boundingBox.max = dimensions * 0.5f;

    sceneObj.maxDistance = getMaxDistance(type);

    objects.push_back(sceneObj);
    totalObjectCount++;
}

vector<GraphicObject> Scene::getVisibleObjects(const Camera& camera)
{
    vector<GraphicObject> visibleObjects;
    renderedObjectCount = 0;

    for (const auto& obj : objects) {
        // Проверка расстояния (LoD)
        float distance = calculateDistanceToCamera(obj, camera);
        if (distance > obj.maxDistance) {
            continue; // Объект слишком далеко
        }

        // Проверка видимости в frustum
        if (!isVisibleInFrustum(obj, camera)) {
            continue; // Объект вне пирамиды видимости
        }

        // Объект видим, добавляем в список
        visibleObjects.push_back(obj.graphicObject);
        renderedObjectCount++;
    }

    return visibleObjects;
}

void Scene::setMaxDistance(ObjectType type, float distance)
{
    maxDistances[static_cast<int>(type)] = distance;
}

float Scene::getMaxDistance(ObjectType type) const
{
    return maxDistances[static_cast<int>(type)];
}

bool Scene::isVisibleInFrustum(const SceneObject& obj, const Camera& camera)
{
    // Создание FrustumCulling для проверки
    FrustumCulling frustum;
    frustum.updateFrustum(camera);

    // Проверка bounding box
    return frustum.isBoundingBoxVisible(obj.boundingBox.min, obj.boundingBox.max,
                                       obj.graphicObject.getModelMatrix());
}

float Scene::calculateDistanceToCamera(const SceneObject& obj, const Camera& camera)
{
    // Получаем позицию камеры и объекта
    vec3 cameraPos = camera.getPosition();
    vec3 objPos = obj.graphicObject.getPosition();

    // Вычисляем расстояние между камерой и объектом
    return length(cameraPos - objPos);
}

bool Scene::lodTest(const GraphicObject& obj, const Camera& camera)
{
    // Вычисляем расстояние
    vec3 cameraPos = camera.getPosition();
    vec3 objPos = obj.getPosition();
    float distance = length(cameraPos - objPos);

    // Проверяем, меньше ли расстояние максимальной дистанции для типа объекта
    return distance <= getMaxDistance(obj.getObjectType());
}

bool Scene::frustumCullingTest(const GraphicObject& obj, const Camera& camera)
{
    // Создание FrustumCulling для проверки
    FrustumCulling frustum;
    frustum.updateFrustum(camera);

    // Создание bounding box на основе dimensions объекта
    vec3 min = -obj.getDimensions() * 0.5f;
    vec3 max = obj.getDimensions() * 0.5f;

    // Проверка bounding box
    return frustum.isBoundingBoxVisible(min, max, obj.getModelMatrix());
}

size_t Scene::getTotalObjectCount() const
{
    return totalObjectCount;
}

size_t Scene::getRenderedObjectCount() const
{
    return renderedObjectCount;
}

GraphicObject Scene::createGraphicObject(const std::string& modelName, const glm::vec3& position, float angle)
{
    // Загружаем models.json для получения информации о модели
    std::ifstream modelsFile("example/DATA/models.json");
    if (!modelsFile.is_open()) {
        std::cerr << "Failed to open models.json" << std::endl;
        return GraphicObject();
    }

    rapidjson::IStreamWrapper isw(modelsFile);
    rapidjson::Document modelsDoc;
    modelsDoc.ParseStream(isw);
    modelsFile.close();

    if (!modelsDoc.HasMember(modelName.c_str())) {
        std::cerr << "Model " << modelName << " not found in models.json" << std::endl;
        return GraphicObject();
    }

    const rapidjson::Value& model = modelsDoc[modelName.c_str()];

    GraphicObject obj;

    // Устанавливаем позицию и угол
    obj.setPosition(position);
    obj.setAngle(angle);

    // Загружаем меш
    if (model.HasMember("mesh")) {
        std::string meshPath = model["mesh"].GetString();
        // Заменяем "data/" на абсолютный путь
        size_t pos = meshPath.find("data/");
        if (pos != std::string::npos) {
            meshPath.replace(pos, 5, "/home/sergey/AGS/build/lab7/example/DATA/");
        }
        // Заменяем "meshes" на "MESHES"
        pos = meshPath.find("meshes");
        if (pos != std::string::npos) {
            meshPath.replace(pos, 6, "MESHES");
        }
        int meshId = ResourceManager::instance().loadMesh(meshPath);
        obj.setMeshId(meshId);
    }

    // Загружаем текстуру
    if (model.HasMember("texture")) {
        std::string texturePath = model["texture"].GetString();
        size_t pos = texturePath.find("data/");
        if (pos != std::string::npos) {
            texturePath.replace(pos, 5, "/home/sergey/AGS/build/lab7/example/DATA/");
        }
        // Заменяем "textures" на "TEXTURES"
        pos = texturePath.find("textures");
        if (pos != std::string::npos) {
            texturePath.replace(pos, 8, "TEXTURES");
        }
        int textureId = ResourceManager::instance().loadTexture(texturePath);
        obj.setTextureId(textureId);
    }

    // Загружаем материал
    if (model.HasMember("material")) {
        std::string materialPath = model["material"].GetString();
        size_t pos = materialPath.find("data/");
        if (pos != std::string::npos) {
            materialPath.replace(pos, 5, "/home/sergey/AGS/build/lab7/example/DATA/");
        }
        // Заменяем "materials" на "MATERIALS"
        pos = materialPath.find("materials");
        if (pos != std::string::npos) {
            materialPath.replace(pos, 9, "MATERIALS");
        }
        int materialId = ResourceManager::instance().loadMaterial(materialPath);
        obj.setMaterialId(materialId);
    }

    // Устанавливаем тип и размеры
    if (model.HasMember("type")) {
        std::string typeStr = model["type"].GetString();
        ObjectType type;
        if (typeStr == "nature") {
            type = ObjectType::SMALL_NATURE;
        } else if (typeStr == "building") {
            type = ObjectType::BIG_PROP;
        } else if (typeStr == "vehicle") {
            type = ObjectType::MEDIUM_PROP;
        } else {
            type = ObjectType::SMALL_PROP;
        }
        obj.setObjectType(type);
    }

    if (model.HasMember("dimensions")) {
        const rapidjson::Value& dims = model["dimensions"];
        vec3 dimensions(dims[0].GetFloat(), dims[1].GetFloat(), dims[2].GetFloat());
        obj.setDimensions(dimensions);
    }

    return obj;
}

bool Scene::loadFromJson(const std::string& sceneFile, const std::string& modelsFile)
{
    // Загружаем сцену
    std::ifstream sceneStream(sceneFile);
    if (!sceneStream.is_open()) {
        std::cerr << "Failed to open scene file: " << sceneFile << std::endl;
        return false;
    }

    rapidjson::IStreamWrapper isw(sceneStream);
    rapidjson::Document sceneDoc;
    sceneDoc.ParseStream(isw);
    sceneStream.close();

    if (!sceneDoc.IsArray()) {
        std::cerr << "Scene file is not an array" << std::endl;
        return false;
    }

    // Очищаем текущую сцену
    objects.clear();
    totalObjectCount = 0;

    for (rapidjson::SizeType i = 0; i < sceneDoc.Size(); ++i) {
        const rapidjson::Value& item = sceneDoc[i];

        if (!item.HasMember("model") || !item.HasMember("position") || !item.HasMember("angle")) {
            std::cerr << "Invalid scene item format" << std::endl;
            continue;
        }

        std::string modelName = item["model"].GetString();
        const rapidjson::Value& pos = item["position"];
        vec3 position(pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat());
        float angle = item["angle"].GetFloat();

        // Создаем объект
        GraphicObject obj = createGraphicObject(modelName, position, angle);

        // Добавляем в сцену
        addObject(obj, obj.getObjectType(), obj.getDimensions());
    }

    return true;
}