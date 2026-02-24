#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace glm;
using namespace std;

Shader shader;

// КОНФИГУРАЦИЯ (легко меняется)
glm::vec2 objOffset(0.0f, 0.0f);     // Начальная позиция
glm::vec2 objSpeed(0.8f, 0.6f);      // Скорость по осям
glm::vec4 red(1.0f, 0.2f, 0.2f, 1.0f); // Цвет 1 (красный)
glm::vec4 blue(0.2f, 0.2f, 1.0f, 1.0f); // Цвет 2 (синий)

// Переменные для расчета времени (Delta Time) и FPS
int lastTime = 0;
int lastFpsTime = 0;
int frameCount = 0;

void drawObject();

void display() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Слегка темный фон
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    shader.activate();

    // Передаем данные в шейдер
    shader.setUniform("offset", objOffset);
    shader.setUniform("color1", blue);
    shader.setUniform("color2", red);

    drawObject();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void simulation() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    
    // Вычисление Delta Time (в секундах)
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // Вычисление FPS
    frameCount++;
    if (currentTime - lastFpsTime >= 1000) { // Обновляем раз в секунду
        float fps = frameCount * 1000.0f / (currentTime - lastFpsTime);
        char title[128];
        snprintf(title, sizeof(title), "Lab 02 - FPS: %.1f", fps);
        glutSetWindowTitle(title);
        
        frameCount = 0;
        lastFpsTime = currentTime;
    }

    // Обновляем позицию (движение не зависит от FPS)
    objOffset += objSpeed * deltaTime;

    // Логика отскока от границ экрана [-1, 1]
    // Так как сам прямоугольник имеет размер от -0.5 до 0.5, 
    // его границы: offset ± 0.5
    float halfSize = 0.5f;
    
    if (objOffset.x + halfSize > 1.0f) {
        objOffset.x = 1.0f - halfSize;
        objSpeed.x = -objSpeed.x;
    } else if (objOffset.x - halfSize < -1.0f) {
        objOffset.x = -1.0f + halfSize;
        objSpeed.x = -objSpeed.x;
    }

    if (objOffset.y + halfSize > 1.0f) {
        objOffset.y = 1.0f - halfSize;
        objSpeed.y = -objSpeed.y;
    } else if (objOffset.y - halfSize < -1.0f) {
        objOffset.y = -1.0f + halfSize;
        objSpeed.y = -objSpeed.y;
    }

    // Запрос на перерисовку кадра
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowPosition(300, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab 02");

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
        return 1;
    }

    printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

    if (!shader.load("shaders/Example.vsh", "shaders/Example.fsh")) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return 1;
    }

    // Инициализируем таймер
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    lastFpsTime = lastTime;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(simulation); // Регистрируем функцию симуляции
    glutMainLoop();
    
    return 0;
}

void drawObject() {
    static bool init = true;
    static GLuint VAO_Index = 0;
    static GLuint VBO_Index = 0;

    if (init) {
        init = false;
        
        glGenVertexArrays(1, &VAO_Index);
        glBindVertexArray(VAO_Index);

        glGenBuffers(1, &VBO_Index);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        
        // Два треугольника, образующие квадрат от -0.5 до 0.5
        GLfloat Verteces[] = {
            -0.5f, +0.5f,
            -0.5f, -0.5f,
            +0.5f, +0.5f,
            
            +0.5f, +0.5f,
            -0.5f, -0.5f,
            +0.5f, -0.5f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindVertexArray(0);
    }

    glBindVertexArray(VAO_Index);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}