#include "RenderManager.h"
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include "ResourceManager.h"
#include "Material.h"
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
	glClearColor(0.5f, 0.6f, 0.8f, 1.0f);
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

		// Передаём параметры ПЕРВОГО света (направление переводим в View Space)
		glm::vec4 lightDirViewSpace = viewMatrix * light.getDirection();
		shaders[0].setUniform("lPosition", lightDirViewSpace);
		shaders[0].setUniform("lAmbient", light.getAmbient());
		shaders[0].setUniform("lDiffuse", light.getDiffuse());
		shaders[0].setUniform("lSpecular", light.getSpecular());

		// Передаём параметры ВТОРОГО света
		glm::vec4 lightDirViewSpace2 = viewMatrix * light2.getDirection();
		shaders[0].setUniform("lPosition2", lightDirViewSpace2);
		shaders[0].setUniform("lAmbient2", light2.getAmbient());
		shaders[0].setUniform("lDiffuse2", light2.getDiffuse());
		shaders[0].setUniform("lSpecular2", light2.getSpecular());

		// Параметры тумана
		shaders[0].setUniform("minDistance", 1.0f);
		shaders[0].setUniform("maxDistance", 100.0f);
		shaders[0].setUniform("fogColor", glm::vec4(0.5f, 0.6f, 0.8f, 1.0f));

		// Отрисовываем все объекты в очереди
		for (const GraphicObject& obj : graphicObjects) {
			// Получаем матрицу модели
			mat4 modelMatrix = obj.getModelMatrix();
			mat4 modelViewMatrix = viewMatrix * modelMatrix;

			// Передаём матрицу вида и модели
			shaders[0].setUniform("modelViewMatrix", modelViewMatrix);

			// Получаем текстуру по ID
			ResourceManager& rm = ResourceManager::instance();
			Texture* texture = rm.getTexture(obj.getTextureId());

			// Если текстура загружена, привязываем её
			if (texture != nullptr) {
				texture->bind(GL_TEXTURE0);
				shaders[0].setUniform("texture_0", 0);
			}

			// Получаем материал по ID и передаём его параметры
			Material* material = rm.getMaterial(obj.getMaterialId());
			if (material != nullptr) {
				shaders[0].setUniform("mAmbient", material->getAmbient());
				shaders[0].setUniform("mDiffuse", material->getDiffuse());
				shaders[0].setUniform("mSpecular", material->getSpecular());
				shaders[0].setUniform("mShininess", material->getShininess());
			}

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

void RenderManager::setLight(const Light& light)
{
	this->light = light;
}

Light& RenderManager::getLight()
{
	return light;
}

void RenderManager::setLight2(const Light& light)
{
	this->light2 = light;
}

Light& RenderManager::getLight2()
{
	return light2;
}
