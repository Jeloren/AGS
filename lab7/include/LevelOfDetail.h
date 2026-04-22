#pragma once
#include <vector>
#include <string>
#include "GraphicObject.h"
#include "Scene.h"

using namespace std;

// Структура для уровня детализации
struct LODLevel {
    string meshPath;      // путь к мешу
    string texturePath;   // путь к текстуре
    float maxDistance;    // максимальная дистанция для этого уровня
    int meshId;          // ID загруженного меша
    int textureId;       // ID загруженной текстуры
};

// Класс для управления уровнями детализации
class LevelOfDetail
{
public:
    // Конструктор
    LevelOfDetail();

    // Добавление уровня детализации для объекта
    void addLODLevel(const string& meshPath, const string& texturePath, float maxDistance);

    // Получение подходящего объекта для заданной дистанции
    GraphicObject getLODObject(float distance, const vec3& position, const vec3& scale, float angle);

    // Загрузка всех уровней детализации
    void loadLevels();

    // Проверка, загружены ли уровни
    bool isLoaded() const { return loaded; }

private:
    vector<LODLevel> lodLevels;
    bool loaded;

    // Сортировка уровней по дистанции (ближайшие сначала)
    void sortLevelsByDistance();
};