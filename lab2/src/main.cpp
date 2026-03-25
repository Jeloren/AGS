#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <vector>
#include <cmath>
using namespace glm;
using namespace std;

Shader shader;

glm::vec2 objOffset(0.0f, 0.0f);     // Начальная позиция
glm::vec2 objSpeed(0.8f, 0.6f);      // Скорость по осям
glm::vec4 red(1.0f, 0.2f, 0.2f, 1.0f); // Цвет 1 (красный)
glm::vec4 blue(0.2f, 0.2f, 1.0f, 1.0f); // Цвет 2 (синий)

// Переменные для расчета времени (Delta Time) и FPS
int lastTime = 0;
int lastFpsTime = 0;
int frameCount = 0;

void drawObject();
void drawFighter();
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

    
    glDisable(GL_DEPTH_TEST); // Отключаем глубину, чтобы не было конфликта
    
    // Передаем ту же самую переменную objOffset, чтобы фигура двигалась с квадратом
    shader.setUniform("offset", objOffset); 
    
    // Цвета для истребителя (светло-серый и темно-серый)
    shader.setUniform("color1", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    shader.setUniform("color2", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    
    drawFighter();

    glEnable(GL_DEPTH_TEST); // Включаем обратно
    
    
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

void drawFighter() {
    static bool init = true;
    static GLuint VAO_Index = 0;
    static GLuint VBO_Index = 0;
    static int vertexCount = 0;

    if (init) {
        init = false;
        std::vector<GLfloat> vertices;

        // Центральный круг
        int segments = 32;
        float radius = 0.15f;
        for (int i = 0; i < segments; i++) {
            float theta1 = 2.0f * 3.1415926f * float(i) / float(segments);
            float theta2 = 2.0f * 3.1415926f * float(i + 1) / float(segments);

            vertices.push_back(0.0f); vertices.push_back(0.0f);
            vertices.push_back(radius * cosf(theta1)); vertices.push_back(radius * sinf(theta1));
            vertices.push_back(radius * cosf(theta2)); vertices.push_back(radius * sinf(theta2));
        }

        // Левое крыло
        float wingWidth = 0.2f;
        float wingHeight = 0.35f;
        
        vertices.push_back(-radius); vertices.push_back(0.0f);
        vertices.push_back(-radius - wingWidth); vertices.push_back(wingHeight);
        vertices.push_back(-radius - wingWidth); vertices.push_back(-wingHeight);

        // Правое крыло
        vertices.push_back(radius); vertices.push_back(0.0f);
        vertices.push_back(radius + wingWidth); vertices.push_back(wingHeight);
        vertices.push_back(radius + wingWidth); vertices.push_back(-wingHeight);

        vertexCount = vertices.size() / 2;

        glGenVertexArrays(1, &VAO_Index);
        glBindVertexArray(VAO_Index);

        glGenBuffers(1, &VBO_Index);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindVertexArray(0);
    }

    glBindVertexArray(VAO_Index);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}