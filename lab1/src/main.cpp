#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace glm;
using namespace std;

Shader shader;

void drawObject();

void display() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    shader.activate();

    drawObject();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void simulation() {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowPosition(300, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("laba_01");

    // Инициализация GLEW (важно для Linux тоже)
    glewExperimental = GL_TRUE; // Часто нужно для core profile
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
        return 1;
    }

    printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

    // Пути к шейдерам относительные
    // Внимание: запускать бинарник надо будет из корня проекта, или поправить путь
    if (!shader.load("shaders/Example.vsh", "shaders/Example.fsh")) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return 1;
    }

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(simulation);
    glutMainLoop();
    return 0;
}

void drawObject() {
    static bool init = true;
    static GLuint VAO_Index = 0;
    static GLuint VBO_Index = 0;

    if (init) {
        init = false;
        glGenBuffers(1, &VBO_Index);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        GLfloat Verteces[] = {
            -0.5f, +0.5f,
            -0.5f, -0.5f,
            +0.5f, +0.5f,
            +0.5f, +0.5f,
            -0.5f, -0.5f,
            +0.5f, -0.5f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO_Index);
        glBindVertexArray(VAO_Index);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    glBindVertexArray(VAO_Index);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}