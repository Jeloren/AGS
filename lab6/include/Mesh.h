#pragma once
#include <GL/glew.h>
#include <iostream>
#include "Vertex.h"
#include <vector>

using namespace std;
// ����� ��� ������ � �����
class Mesh
{
public:
	// �����������
	Mesh();
	// �������� ���� �� �������� obj-�����
	bool load(const string& filename);
	// ����� ����
	void drawOne();
private:
	// ������ VAO-�������
	GLuint vao;
	// ������ VBO-������ ������
	GLuint vertexBuffer;
	// ������ VBO-������ ��������
	GLuint indexBuffer;
	// ���������� ������ � ����
	int vertexCount;

	// �������� ������ �� obj-�����
	bool loadObjFile(const std::string& filename, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

	// �������� ������� OpenGL
	void createBuffers(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
};