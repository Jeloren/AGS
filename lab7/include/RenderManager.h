#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"
#include "GraphicObject.h"
#include "Texture.h"
#include "Light.h"

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
	// Добавление объекта в очередь отрисовки
	void addToRenderQueue(const GraphicObject& obj);
	// Отрисовка всех объектов из очереди
	void finish();
	// Установка первого источника света
	void setLight(const Light& light);
	// Получение первого источника света
	Light& getLight();
	// Установка второго источника света
	void setLight2(const Light& light);
	// Получение второго источника света
	Light& getLight2();

	// Получение количества смен материалов
	int getMaterialSwitches() const;
	// Получение количества смен текстур
	int getTextureSwitches() const;
	// Получение количества drawcall'ов
	int getDrawCalls() const;

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
	// Первый источник света
	Light light;
	// Второй источник света
	Light light2;
	// Очередь объектов для отрисовки
	vector<GraphicObject> graphicObjects;
	// Счетчики для оптимизации
	int materialSwitches;
	int textureSwitches;
	int drawCalls;
};
