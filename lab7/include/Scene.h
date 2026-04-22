#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "GraphicObject.h"
#include "Camera.h"

using namespace std;
using namespace glm;

// Структура для описания ограничивающего объёма
struct BoundingBox {
    vec3 min;
    vec3 max;
    vec3 dimensions; // размеры (длина, ширина, высота)
};

// Класс для управления сценой с LoD и Frustum Culling
class Scene
{
public:
    // Конструктор
    Scene();

    // Добавление объекта в сцену
    void addObject(const GraphicObject& obj, ObjectType type, const vec3& dimensions);

    // Получение видимых объектов с учётом LoD и Frustum Culling
    vector<GraphicObject> getVisibleObjects(const Camera& camera);

    // Установка максимальных дистанций для типов объектов
    void setMaxDistance(ObjectType type, float distance);

    // Получение максимальной дистанции для типа объекта
    float getMaxDistance(ObjectType type) const;

    // Загрузка сцены из JSON файла
    bool loadFromJson(const std::string& sceneFile, const std::string& modelsFile);

    // Создание графического объекта на основе описания из JSON
    GraphicObject createGraphicObject(const std::string& modelName, const glm::vec3& position, float angle);

    // Проверка необходимости отрисовки объекта (LoD тест)
    bool lodTest(const GraphicObject& obj, const Camera& camera);

    // Проверка попадания объекта в усеченную пирамиду видимости
    bool frustumCullingTest(const GraphicObject& obj, const Camera& camera);

    // Получение общего количества объектов сцены
    size_t getTotalObjectCount() const;

    // Получение количества объектов, переданных на отрисовку
    size_t getRenderedObjectCount() const;

private:
    // Структура для хранения объекта сцены
    struct SceneObject {
        GraphicObject graphicObject;
        ObjectType type;
        BoundingBox boundingBox;
        float maxDistance;
    };

    vector<SceneObject> objects;

    // Максимальные дистанции для каждого типа объектов
    float maxDistances[4] = {
        500.0f, // SMALL_NATURE
        400.0f, // BIG_PROP
        300.0f, // MEDIUM_PROP
        200.0f  // SMALL_PROP
    };

    // Счетчики объектов
    size_t totalObjectCount;
    size_t renderedObjectCount;

    // Проверка видимости объекта через Frustum Culling
    bool isVisibleInFrustum(const SceneObject& obj, const Camera& camera);

    // Вычисление расстояния от камеры до объекта
    float calculateDistanceToCamera(const SceneObject& obj, const Camera& camera);
};