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
#include "Light.h"
#include "Scene.h"


// используемые пространства имен (для удобства)
using namespace glm;
using namespace std;

bool rightMouseButtonPressed = false;
int lastX = 0, lastY = 0;

// состояние клавиш WASD
bool keyState[256] = {false};

static float simulationTime = 0.0f;
static float deltaTime = 0.0f;

// Сцена с поддержкой LoD и Frustum Culling
Scene scene;

// вспомогательная функция для инициализации графических объектов
void initGraphicObjects()
{
    // Загружаем большую сцену из JSON
    if (!scene.loadFromJson("example/DATA/SCENES/big_scene.json", "example/DATA/models.json")) {
        std::cerr << "Failed to load scene from JSON" << std::endl;
        // Fallback to manual loading if needed
    }
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

	// Получаем видимые объекты из сцены с учётом LoD и Frustum Culling
	vector<GraphicObject> visibleObjects = scene.getVisibleObjects(renderManager.getCamera());

	// Добавляем видимые объекты в очередь отрисовки
	for (const GraphicObject& obj : visibleObjects) {
		renderManager.addToRenderQueue(obj);
	}

	// Завершаем отрисовку всех объектов
	renderManager.finish();

	// Меняем передний и задний буферы цвета
	glutSwapBuffers();

	// Вычисление FPS и его вывод в заголовок окна
	char windowTitle[256];
	int FPS = getFps();
	size_t totalObjects = scene.getTotalObjectCount();
	size_t renderedObjects = scene.getRenderedObjectCount();
	int materialSwitches = RenderManager::instance().getMaterialSwitches();
	int textureSwitches = RenderManager::instance().getTextureSwitches();
	int drawCalls = RenderManager::instance().getDrawCalls();
	snprintf(windowTitle, sizeof(windowTitle), "Laba_07 [%i FPS] Total: %zu Rendered: %zu MatSw: %i TexSw: %i DrawCalls: %i", 
	         FPS, totalObjects, renderedObjects, materialSwitches, textureSwitches, drawCalls);
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
	if (keyState['W'] || keyState['w']) {
		camera.moveOXZ(0.0f, moveSpeed); // Движение вперед
	}
	if (keyState['S'] || keyState['s']) {
		camera.moveOXZ(0.0f, -moveSpeed); // Движение назад
	}
	if (keyState['A'] || keyState['a']) {
		camera.moveOXZ(-moveSpeed, 0.0f); // Движение влево
	}
	if (keyState['D'] || keyState['d']) {
		camera.moveOXZ(moveSpeed, 0.0f); // Движение вправо
	}
	if (keyState['Q'] || keyState['q']) {
		camera.moveVertical(moveSpeed); // Движение вверх
	}
	if (keyState['E'] || keyState['e']) {
		camera.moveVertical(-moveSpeed); // Движение вниз
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
	glutCreateWindow("laba_06");
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
	
	// Инициализируем первый источник света (основной, БЕЛЫЙ, спереди)
	Light light;
	light.setDirection(glm::vec3(0.0f, -1.0f, 1.0f));  // Вперед и вниз (основное освещение)
	light.setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	light.setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));  // Белый
	light.setSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));  // Белый блик
	RenderManager::instance().setLight(light);
	
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