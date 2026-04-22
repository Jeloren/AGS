#include "FrustumCulling.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

using namespace glm;

FrustumCulling::FrustumCulling()
{
    // Инициализация плоскостей
    for (int i = 0; i < 6; ++i) {
        planes[i] = Plane();
    }
}

void FrustumCulling::updateFrustum(const Camera& camera)
{
    // Получение матрицы проекции * вид
    mat4 viewProjection = camera.getProjectionMatrix() * camera.getViewMatrix();

    // Извлечение плоскостей
    extractPlanes(viewProjection);
}

void FrustumCulling::extractPlanes(const mat4& viewProjectionMatrix)
{
    // Извлечение плоскостей из матрицы viewProjection
    // Порядок: left, right, bottom, top, near, far

    // Left plane
    planes[0].normal.x = viewProjectionMatrix[0][3] + viewProjectionMatrix[0][0];
    planes[0].normal.y = viewProjectionMatrix[1][3] + viewProjectionMatrix[1][0];
    planes[0].normal.z = viewProjectionMatrix[2][3] + viewProjectionMatrix[2][0];
    planes[0].distance = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][0];

    // Right plane
    planes[1].normal.x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][0];
    planes[1].normal.y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][0];
    planes[1].normal.z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][0];
    planes[1].distance = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][0];

    // Bottom plane
    planes[2].normal.x = viewProjectionMatrix[0][3] + viewProjectionMatrix[0][1];
    planes[2].normal.y = viewProjectionMatrix[1][3] + viewProjectionMatrix[1][1];
    planes[2].normal.z = viewProjectionMatrix[2][3] + viewProjectionMatrix[2][1];
    planes[2].distance = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][1];

    // Top plane
    planes[3].normal.x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][1];
    planes[3].normal.y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][1];
    planes[3].normal.z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][1];
    planes[3].distance = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][1];

    // Near plane
    planes[4].normal.x = viewProjectionMatrix[0][3] + viewProjectionMatrix[0][2];
    planes[4].normal.y = viewProjectionMatrix[1][3] + viewProjectionMatrix[1][2];
    planes[4].normal.z = viewProjectionMatrix[2][3] + viewProjectionMatrix[2][2];
    planes[4].distance = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][2];

    // Far plane
    planes[5].normal.x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][2];
    planes[5].normal.y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][2];
    planes[5].normal.z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][2];
    planes[5].distance = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][2];

    // Нормализация плоскостей
    for (int i = 0; i < 6; ++i) {
        float length = glm::length(planes[i].normal);
        if (length > 0.0f) {
            planes[i].normal /= length;
            planes[i].distance /= length;
        }
    }
}

bool FrustumCulling::isBoundingBoxVisible(const vec3& min, const vec3& max, const mat4& modelMatrix) const
{
    // Получение 8 вершин bounding box в мировых координатах
    vec4 vertices[8] = {
        modelMatrix * vec4(min.x, min.y, min.z, 1.0f),
        modelMatrix * vec4(max.x, min.y, min.z, 1.0f),
        modelMatrix * vec4(min.x, max.y, min.z, 1.0f),
        modelMatrix * vec4(max.x, max.y, min.z, 1.0f),
        modelMatrix * vec4(min.x, min.y, max.z, 1.0f),
        modelMatrix * vec4(max.x, min.y, max.z, 1.0f),
        modelMatrix * vec4(min.x, max.y, max.z, 1.0f),
        modelMatrix * vec4(max.x, max.y, max.z, 1.0f)
    };

    // Проверка для каждой плоскости
    for (int i = 0; i < 6; ++i) {
        bool allOutside = true;

        // Проверяем все вершины
        for (int j = 0; j < 8; ++j) {
            vec3 vertex = vec3(vertices[j]);
            if (planes[i].distanceToPoint(vertex) >= 0.0f) {
                allOutside = false;
                break;
            }
        }

        // Если все вершины вне плоскости, то bounding box невидим
        if (allOutside) {
            return false;
        }
    }

    return true;
}

bool FrustumCulling::isPointVisible(const vec3& point) const
{
    // Проверка точки со всеми плоскостями
    for (int i = 0; i < 6; ++i) {
        if (planes[i].distanceToPoint(point) < 0.0f) {
            return false; // Точка вне плоскости
        }
    }
    return true;
}