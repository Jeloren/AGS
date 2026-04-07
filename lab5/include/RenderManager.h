#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"
#include "GraphicObject.h"
#include "Texture.h"

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
};
