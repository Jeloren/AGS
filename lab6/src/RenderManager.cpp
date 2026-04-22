#include "RenderManager.h"
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include "ResourceManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;
using namespace glm;

void RenderManager::init()
{
	// Инициализируем DevIL
	ilInit();

	// Загружаем шейдер для текстурирования
	Shader shader;
	if (!shader.load("shaders/Example.vsh", "shaders/Example.fsh")) {
		cerr << "Failed to load shader\n";
		return;
	}
	shaders.push_back(shader);

	// Инициализируем камеру
	camera.setProjectionMatrix(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	// Камера уже хорошо позиционирована в конструкторе Camera
	// Не нужны moveOXZ и zoom

	// Инициализируем OpenGL параметры
	glClearColor(0.5f, 0.5f, 0.55f, 1.0f); // Средне-серый фон для контраста со светлыми объектами
	glEnable(GL_DEPTH_TEST);
}

void RenderManager::start()
{
	// Очищаем буфер цвета и глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Очищаем очередь объектов для нового кадра
	graphicObjects.clear();
}

void RenderManager::setCamera(const Camera& cam)
{
	this->camera = cam;
}

Camera& RenderManager::getCamera()
{
	return camera;
}

void RenderManager::setLight(const vec3& position, const vec3& ambient, const vec3& diffuse, const vec3& specular)
{
	lightPosition = position;
	lightAmbient = ambient;
	lightDiffuse = diffuse;
	lightSpecular = specular;
}

void RenderManager::setLight2(const vec3& position, const vec3& ambient, const vec3& diffuse, const vec3& specular)
{
	lightPosition2 = position;
	lightAmbient2 = ambient;
	lightDiffuse2 = diffuse;
	lightSpecular2 = specular;
}

void RenderManager::addToRenderQueue(const GraphicObject& obj)
{
	graphicObjects.push_back(obj);
}

void RenderManager::finish()
{
	// Активируем шейдер для отрисовки
	if (shaders.size() > 0) {
		shaders[0].activate();

		// Передаём матрицы проекции и вида в шейдер
		mat4 projectionMatrix = camera.getProjectionMatrix();
		mat4 viewMatrix = camera.getViewMatrix();

		shaders[0].setUniform("projectionMatrix", projectionMatrix);

		// Отрисовываем все объекты в очереди
		for (const GraphicObject& obj : graphicObjects) {
			// Получаем матрицу модели
			mat4 modelMatrix = obj.getModelMatrix();
			mat4 modelViewMatrix = viewMatrix * modelMatrix;

			// Передаём матрицу вида и модели
			shaders[0].setUniform("modelViewMatrix", modelViewMatrix);

			// Передаём цвет
			vec4 color = obj.getColor();
			shaders[0].setUniform("color", color);

			// Получаем текстуру по ID
			ResourceManager& rm = ResourceManager::instance();
			Texture* texture = rm.getTexture(obj.getTextureId());

			// Если текстура загружена, привязываем её
			if (texture != nullptr) {
				texture->bind(GL_TEXTURE0);
				shaders[0].setUniform("texture_0", 0);
			}

			// Получаем материал по ID
			Material* material = rm.getMaterial(obj.getMaterialId());
			if (material != nullptr) {
				shaders[0].setUniform("materialAmbient", material->getAmbient());
				shaders[0].setUniform("materialDiffuse", material->getDiffuse());
				shaders[0].setUniform("materialSpecular", material->getSpecular());
				shaders[0].setUniform("materialShininess", material->getShininess());
			}

			// Передаём параметры источника света
			shaders[0].setUniform("lightPosition", lightPosition);
			shaders[0].setUniform("lightAmbient", lightAmbient);
			shaders[0].setUniform("lightDiffuse", lightDiffuse);
			shaders[0].setUniform("lightSpecular", lightSpecular);

			// Передаём параметры второго источника света
			shaders[0].setUniform("lightPosition2", lightPosition2);
			shaders[0].setUniform("lightAmbient2", lightAmbient2);
			shaders[0].setUniform("lightDiffuse2", lightDiffuse2);
			shaders[0].setUniform("lightSpecular2", lightSpecular2);

			// Матрица нормалей для корректного преобразования нормалей
			mat3 normalMatrix = mat3(transpose(inverse(modelViewMatrix)));
			shaders[0].setUniform("normalMatrix", normalMatrix);

			// Получаем меш и отрисовываем его
			Mesh* mesh = rm.getMesh(obj.getMeshId());
			if (mesh != nullptr) {
				mesh->drawOne();
			}
		}

		// Деактивируем шейдер
		Shader::deactivate();
	}
}
