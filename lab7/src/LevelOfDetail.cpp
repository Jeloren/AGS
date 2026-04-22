#include "LevelOfDetail.h"
#include "ResourceManager.h"
#include <algorithm>

using namespace std;

LevelOfDetail::LevelOfDetail() : loaded(false)
{
}

void LevelOfDetail::addLODLevel(const string& meshPath, const string& texturePath, float maxDistance)
{
    LODLevel level;
    level.meshPath = meshPath;
    level.texturePath = texturePath;
    level.maxDistance = maxDistance;
    level.meshId = -1;
    level.textureId = -1;

    lodLevels.push_back(level);
    sortLevelsByDistance();
}

GraphicObject LevelOfDetail::getLODObject(float distance, const vec3& position, const vec3& scale, float angle)
{
    if (!loaded || lodLevels.empty()) {
        // Возвращаем пустой объект, если уровни не загружены
        return GraphicObject();
    }

    // Находим подходящий уровень детализации
    for (const auto& level : lodLevels) {
        if (distance <= level.maxDistance) {
            GraphicObject obj;
            obj.setMeshId(level.meshId);
            obj.setTextureId(level.textureId);
            obj.setPosition(position);
            obj.setScale(scale.x, scale.y, scale.z); // Используем компоненты vec3
            obj.setAngle(angle);
            obj.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
            return obj;
        }
    }

    // Если расстояние слишком большое, возвращаем самый низкий уровень детализации
    const auto& level = lodLevels.back();
    GraphicObject obj;
    obj.setMeshId(level.meshId);
    obj.setTextureId(level.textureId);
    obj.setPosition(position);
    obj.setScale(scale.x, scale.y, scale.z); // Используем компоненты vec3
    obj.setAngle(angle);
    obj.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    return obj;
}

void LevelOfDetail::loadLevels()
{
    if (loaded) return;

    ResourceManager& rm = ResourceManager::instance();

    for (auto& level : lodLevels) {
        if (!level.meshPath.empty()) {
            level.meshId = rm.loadMesh(level.meshPath);
        }
        if (!level.texturePath.empty()) {
            level.textureId = rm.loadTexture(level.texturePath);
        }
    }

    loaded = true;
}

void LevelOfDetail::sortLevelsByDistance()
{
    // Сортировка по возрастанию дистанции (ближайшие уровни первыми)
    sort(lodLevels.begin(), lodLevels.end(),
         [](const LODLevel& a, const LODLevel& b) {
             return a.maxDistance < b.maxDistance;
         });
}