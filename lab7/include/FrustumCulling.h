#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"

using namespace glm;

// Структура для представления плоскости (для frustum)
struct Plane {
    vec3 normal;
    float distance;

    Plane() : normal(0.0f), distance(0.0f) {}
    Plane(const vec3& n, float d) : normal(n), distance(d) {}

    // Расстояние от точки до плоскости
    float distanceToPoint(const vec3& point) const {
        return dot(normal, point) + distance;
    }
};

// Класс для усечения по пирамиде видимости
class FrustumCulling
{
public:
    // Конструктор
    FrustumCulling();

    // Обновление frustum на основе камеры
    void updateFrustum(const Camera& camera);

    // Проверка видимости ограничивающего объёма
    bool isBoundingBoxVisible(const vec3& min, const vec3& max, const mat4& modelMatrix) const;

    // Проверка видимости точки
    bool isPointVisible(const vec3& point) const;

private:
    // 6 плоскостей frustum: left, right, bottom, top, near, far
    Plane planes[6];

    // Извлечение плоскостей из матрицы проекции * вид
    void extractPlanes(const mat4& viewProjectionMatrix);
};