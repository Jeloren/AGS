#include "stdio.h"            // для printf/sprintf
#include "GL/glew.h"
#include <GL/glut.h>          // GLUT для кроссплатформенного ввода/окна
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "GraphicObject.h"
#include "ResourceManager.h"
#include "RenderManager.h"


// используемые пространства имен (для удобства)
using namespace glm;
using namespace std;

bool rightMouseButtonPressed = false;
int lastX = 0, lastY = 0;

// состояние клавиш WASD
bool keyState[256] = {false};

static float simulationTime = 0.0f;
static float deltaTime = 0.0f;

// Вспомогательный вектор для хранения графических объектов
vector<GraphicObject> graphicObjects;

// вспомогательная функция для инициализации графических объектов
// вспомогательная функция для инициализации графических объектов
void initGraphicObjects()
{
    ResourceManager& rm = ResourceManager::instance();
    int meshId = -1;
    int textureId = -1;
    GraphicObject graphicObject;

    // Дом (сдвинут по диагонали, цвет - темный кирпич)
    meshId = rm.loadMesh("example/MESHES/buildings/house_2.obj");
    textureId = rm.loadTexture("example/TEXTURES/buildings/house_2_red.png");
    graphicObject.setMeshId(meshId);
    graphicObject.setTextureId(textureId);
    graphicObject.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    graphicObject.setPosition(vec3(5.0f, 2.0f, -5.0f));
    graphicObject.setAngle(45.0f);
    graphicObjects.push_back(graphicObject);

    // Дерево 1
    meshId = rm.loadMesh("example/MESHES/natures/big_tree.obj");
    textureId = rm.loadTexture("example/TEXTURES/natures/nature.png");
    graphicObject.setMeshId(meshId);
    graphicObject.setTextureId(textureId);
    graphicObject.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    graphicObject.setPosition(vec3(3.0f, 1.0f, -8.0f));
    graphicObject.setAngle(15.0f);
    graphicObjects.push_back(graphicObject);

    // Дерево 2
    meshId = rm.loadMesh("example/MESHES/natures/big_tree.obj");
    textureId = rm.loadTexture("example/TEXTURES/natures/nature.png");
    graphicObject.setMeshId(meshId);
    graphicObject.setTextureId(textureId);
    graphicObject.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    graphicObject.setPosition(vec3(8.0f, 1.0f, -2.0f));
    graphicObject.setAngle(120.0f);
    graphicObjects.push_back(graphicObject);

    // Дерево 3
    meshId = rm.loadMesh("example/MESHES/natures/big_tree.obj");
    textureId = rm.loadTexture("example/TEXTURES/natures/nature.png");
    graphicObject.setMeshId(meshId);
    graphicObject.setTextureId(textureId);
    graphicObject.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    graphicObject.setPosition(vec3(-4.0f, 1.0f, -6.0f));
    graphicObject.setAngle(40.0f);
    graphicObjects.push_back(graphicObject);

    // Джип 1 (желтый, развернут)
    meshId = rm.loadMesh("example/MESHES/vehicles/jeep.obj");
    textureId = rm.loadTexture("example/TEXTURES/vehicles/jeep_brown.png");
    graphicObject.setMeshId(meshId);
    graphicObject.setTextureId(textureId);
    graphicObject.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    graphicObject.setPosition(vec3(-2.0f, 0.0f, 5.0f));
    graphicObject.setAngle(135.0f);
    graphicObjects.push_back(graphicObject);

    // Джип 2 (бирюзовый)
    meshId = rm.loadMesh("example/MESHES/vehicles/jeep.obj");
    textureId = rm.loadTexture("example/TEXTURES/vehicles/jeep_green.png");
    graphicObject.setMeshId(meshId);
    graphicObject.setTextureId(textureId);
    graphicObject.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    graphicObject.setPosition(vec3(2.0f, 0.0f, 6.0f));
    graphicObject.setAngle(-20.0f);
    graphicObjects.push_back(graphicObject);

    // Полицейская машина (фиолетовая, стоит по центру)
    meshId = rm.loadMesh("example/MESHES/vehicles/police_car.obj");
    textureId = rm.loadTexture("example/TEXTURES/vehicles/police_car.png");
    graphicObject.setMeshId(meshId);
    graphicObject.setTextureId(textureId);
    graphicObject.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    graphicObject.setPosition(vec3(0.0f, 0.0f, 0.0f));
    graphicObject.setAngle(90.0f);
    graphicObjects.push_back(graphicObject);

	meshId = rm.loadMesh("example/MESHES/characters/E 45 Aircraft_obj.obj");
	textureId = rm.loadTexture("example/TEXTURES/characters/E-45 _col.jpg");
    graphicObject.setMeshId(meshId);
	graphicObject.setTextureId(textureId);
    graphicObject.setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f)); 
    graphicObject.setPosition(vec3(0.0f, 5.0f, 15.0f)); 
    graphicObject.setAngle(0.0f);
 

    graphicObjects.push_back(graphicObject);

}

// Функция получения времени симуляции
float getSimulationTime() {
	static int startTime = glutGet(GLUT_ELAPSED_TIME); // В миллисекундах
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	return (currentTime - startTime) / 1000.0f; // Переводим в секунды
}

int getFps() {
	static int frameCount = 0;
	static float lastTime = 0.0f;
	static int fps = 0;

	float currentTime = getSimulationTime();
	frameCount++;

	if (currentTime - lastTime >= 1.0f) {
		fps = frameCount;
		frameCount = 0;
		lastTime = currentTime;
	}
	return fps;
}

void display()
{
	// Получаем инстанс RenderManager
	RenderManager& renderManager = RenderManager::instance();
	
	// Начинаем новый кадр
	renderManager.start();
	
	// Добавляем все графические объекты в очередь отрисовки
	for (const GraphicObject& obj : graphicObjects) {
		renderManager.addToRenderQueue(obj);
	}
	
	// Завершаем отрисовку всех объектов
	renderManager.finish();
	
	// Меняем передний и задний буферы цвета
	glutSwapBuffers();
	
	// Вычисление FPS и его вывод в заголовок окна
	char windowTitle[128];
	int FPS = getFps();
	snprintf(windowTitle, sizeof(windowTitle), "Laba_05 [%i FPS]", FPS);
	glutSetWindowTitle(windowTitle);
}

// функция, вызываемая при изменении размеров окна
void reshape(int w, int h)
{
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, w, h);
}

// обработчики ввода
void keyboardDown(unsigned char key, int x, int y) {
	keyState[key] = true;
}
void keyboardUp(unsigned char key, int x, int y) {
	keyState[key] = false;
}
void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			rightMouseButtonPressed = true;
			lastX = x;
			lastY = y;
		} else {
			rightMouseButtonPressed = false;
		}
	}
}

void mouseMotion(int x, int y) {
	if (rightMouseButtonPressed) {
		float dx = x - lastX;
		float dy = y - lastY;
		float rotateSpeed = 0.5f;
		RenderManager& renderManager = RenderManager::instance();
		renderManager.getCamera().rotate(dx * rotateSpeed, dy * rotateSpeed);
		lastX = x;
		lastY = y;
	}
}

// функция вызывается, когда процессор простаивает, т.е. максимально часто
void simulation() {
	static float lastTime = 0.0f;
	float currentTime = getSimulationTime();

	// Вычисляем deltaTime
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	float moveSpeed = 20.0f * deltaTime; // Скорость перемещения камеры
	
	RenderManager& renderManager = RenderManager::instance();
	Camera& camera = renderManager.getCamera();

	// Обработка перемещения камеры с клавиатуры
	if (keyState['W']) {
		camera.moveOXZ(0.0f, moveSpeed); // Движение вперед
	}
	if (keyState['S']) {
		camera.moveOXZ(0.0f, -moveSpeed); // Движение назад
	}
	if (keyState['A']) {
		camera.moveOXZ(-moveSpeed, 0.0f); // Движение влево
	}
	if (keyState['D']) {
		camera.moveOXZ(moveSpeed, 0.0f); // Движение вправо
	}

	// вращение обрабатывается в callback-е mouseMotion

	// Установка признака необходимости перерисовать окно
	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y)
{
	// Определяем, на сколько необходимо приблизить/удалить камеру
	float delta = 2.0f; // Шаг изменения расстояния
	
	RenderManager& renderManager = RenderManager::instance();
	Camera& camera = renderManager.getCamera();
	
	if (direction > 0)
	{
		// Вращение колесика вверх (приближение)
		camera.zoom(+delta); // Уменьшаем расстояние до цели
	}
	else
	{
		// Вращение колесика вниз (удаление)
		camera.zoom(-delta); // Увеличиваем расстояние до цели
	}
}

// основная функция
int main(int argc, char** argv)
{
	// инициализация библиотеки GLUT
	glutInit(&argc, argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH
		| GLUT_STENCIL | GLUT_MULTISAMPLE);
	// требования к версии OpenGL (версия 3.3 без поддержки обратной совместимости)
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	// устанавливаем верхний левый угол окна
	glutInitWindowPosition(300, 100);
	// устанавливаем размер окна
	glutInitWindowSize(800, 600);
	// создание окна
	glutCreateWindow("laba_05");
	// инициализация GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return 0;
	}
	// определение текущей версии OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));
	
	// Инициализируем RenderManager
	RenderManager::instance().init();
	
	// Инициализируем графические объекты
	initGraphicObjects();
	// устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(display);
	// устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(reshape);
	// устанавливаем функцию, которая вызывается всякий раз, когда процессор простаивает
	glutIdleFunc(simulation);
	glutMouseWheelFunc(mouseWheel);
	// регистрируем обработчики ввода
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);

	// основной цикл обработки сообщений ОС
	glutMainLoop();

	return 0;
}