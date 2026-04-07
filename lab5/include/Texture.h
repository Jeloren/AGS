#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>

using namespace std;

class Texture
{
public:
	// Конструктор
	Texture();
	// Загрузка текстуры из файла с использованием DevIL
	void load(const string& filename);
	// Привязка текстуры к нужному текстурному блоку
	void bind(GLenum texUnit = GL_TEXTURE0);

private:
	// Индекс (идентификатор) текстуры OpenGL
	GLuint texIndex;
};
