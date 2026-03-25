#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "GraphicObject.h"

// Внешняя функция для отрисовки геометрии куба
extern void drawBox();

Shader shader;
Camera camera;
// Используем одно имя вектора для всех объектов
std::vector<GraphicObject> graphicObjects;

int windowWidth = 800;
int windowHeight = 600;

// Переменные для мыши
bool isRMBPressed = false;
int lastMouseX = 0;
int lastMouseY = 0;

void initApp() {
    int count = 8;
    float radius = 4.0f;
    for (int i = 0; i < count; i++) {
        float angle = 2.0f * 3.14159265f * i / count;
        glm::vec3 pos(radius * cos(angle), 0.0f, radius * sin(angle));
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        float rotAngle = 0.0f;
        GraphicObject obj(pos, rotAngle, color);
        graphicObjects.push_back(obj);
    }

    
    // Задаем размер кубиков флага (чтобы они отличались от остальных)
    float flagCubeSize = 0.5f; 
    
    // Вычисляем начальные координаты, чтобы флаг стоял ровно по центру (0,0,0)
    float startX = - (4 * flagCubeSize) / 2.0f + (flagCubeSize / 2.0f);
    float startY = - (3 * flagCubeSize) / 2.0f + (flagCubeSize / 2.0f);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 4; col++) {
            // Флаг стоит вертикально: меняем X и Y. Z = 0 (в центре кольца)
            glm::vec3 pos(startX + col * flagCubeSize, startY + row * flagCubeSize, 0.0f);
            
            glm::vec4 color;
            // Определяем цвет в зависимости от ряда (снизу вверх)
            if (row == 0) {
                color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Красный (низ)
            } else if (row == 1) {
                color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Синий (середина)
            } else {
                color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f); // Светло-серый, вместо белого, чтобы было видно на фоне
            }

            GraphicObject obj(pos, 0.0f, color);
            
            // ВАЖНО: Применяем размер (масштаб) к объекту.
            // Если в вашем классе GraphicObject есть метод setScale, раскомментируйте строку ниже:
            obj.setScale(glm::vec3(flagCubeSize, flagCubeSize, flagCubeSize));
            
            graphicObjects.push_back(obj);
        }
    }
}

void display() {
    // 1. Белый фон
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // 2. Режим линий (делает кубики "прозрачными" каркасами)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    
    // 3. Отсечение задних граней (чтобы не видеть внутренние ребра задних стенок)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    shader.activate();

    // Получаем матрицы из камеры
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projectionMatrix = camera.getProjectionMatrix(windowWidth, windowHeight);
    
    // Передаем проекцию один раз на кадр
    shader.setUniform("projectionMatrix", projectionMatrix);

    // Отрисовка всех объектов
    for (auto& grObj : graphicObjects) {
        // Формируем Model-View матрицу для конкретного объекта
        glm::mat4 modelViewMatrix = viewMatrix * grObj.getModelMatrix();
        
        shader.setUniform("modelViewMatrix", modelViewMatrix);
        shader.setUniform("color", grObj.getColor());
        
        drawBox(); 
    }
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC
}

// Управление центром камеры (куда смотрим) стрелками
void specialKeys(int key, int x, int y) {
    float moveSpeed = 0.2f;
    switch (key) {
        case GLUT_KEY_UP:    camera.move(0.0f, moveSpeed);  break;
        case GLUT_KEY_DOWN:  camera.move(0.0f, -moveSpeed); break;
        case GLUT_KEY_LEFT:  camera.move(-moveSpeed, 0.0f); break;
        case GLUT_KEY_RIGHT: camera.move(moveSpeed, 0.0f);  break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    // Правая кнопка мыши для вращения
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isRMBPressed = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isRMBPressed = false;
        }
    }
    
    // Зум колесиком (в freeglut это кнопки 3 и 4)
    if (state == GLUT_DOWN) {
        if (button == 3) camera.zoom(-0.5f);
        if (button == 4) camera.zoom(0.5f);
    }
    
    glutPostRedisplay();
}

void motion(int x, int y) {
    if (isRMBPressed) {
        float dx = (x - lastMouseX) * 0.005f; 
        float dy = (y - lastMouseY) * 0.005f; 
        
        camera.rotate(dx, dy); 
        
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    // Важно: добавить GLUT_DEPTH для корректного 3D
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lab 03 - 3D Circle Scene");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return 1;
    }

    // Загружаем шейдеры
    if (!shader.load("shaders/Example.vsh", "shaders/Example.fsh")) {
        return 1;
    }

    // Создаем кубики по кругу
    initApp();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    // Запускаем цикл
    glutMainLoop();
    return 0;
}