#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"
#include "GraphicObject.h"
#include "Texture.h"
#include "Material.h"

using namespace std;
using namespace glm;

class RenderManager
{
public:
	// Метод для получения единственного экземпляра (Singleton)
	static RenderManager& instance()
	{
		static RenderManager RenderManager;
		return RenderManager;
	}

	// Инициализация OpenGL параметров и загрузка шейдеров
	void init();
	// Начало нового кадра: очистка экрана и очистка очереди объектов
	void start();
	// Установка камеры
	void setCamera(const Camera& cam);
	// Получение камеры
	Camera& getCamera();
	// Граница между публичными и приватными методами
	// Установка источника света
	void setLight(const vec3& position, const vec3& ambient, const vec3& diffuse, const vec3& specular);
	// Установка второго источника света
	void setLight2(const vec3& position, const vec3& ambient, const vec3& diffuse, const vec3& specular);
	// Добавление объекта в очередь отрисовки
	void addToRenderQueue(const GraphicObject& obj);
	// Отрисовка всех объектов из очереди
	void finish();

private:
	// Приватный конструктор (Singleton)
	RenderManager() {}
	// Удаляем конструктор копирования
	RenderManager(const RenderManager& v) = delete;
	// Удаляем оператор присваивания
	RenderManager& operator=(const RenderManager& v) = delete;

private:
	// Вектор шейдеров
	vector<Shader> shaders;
	// Камера
	Camera camera;
	// Очередь объектов для отрисовки
	vector<GraphicObject> graphicObjects;
	// Источник света 1
	vec3 lightPosition;
	vec3 lightAmbient;
	vec3 lightDiffuse;
	vec3 lightSpecular;
	// Источник света 2
	vec3 lightPosition2;
	vec3 lightAmbient2;
	vec3 lightDiffuse2;
	vec3 lightSpecular2;
};
