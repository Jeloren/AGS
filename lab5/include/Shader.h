#pragma once
#include <string>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <map>

using namespace std;
using namespace glm;

class Shader
{
public:
	// �������� ������� �� ������� ������
	bool load(string vertexShaderFilename, string fragmentShaderFilename);
	// ����� ������� � �������� ��������
	void activate();
	// ���������� �������
	static void deactivate();
	// ��������� uniform-����������
	void setUniform(std::string name, int value);
	void setUniform(std::string name, float value);
	void setUniform(std::string name, glm::vec2& value);
	void setUniform(std::string name, glm::vec4& value);
	void setUniform(std::string name, glm::mat4& value);
private:
	// �������� ���������� ������� ���������� ����
	// � �������� ��������� ������ ������� �� ���������� �����
	GLuint createShaderObject(GLenum shaderType, string filename);

	// ��������� ������� (location) uniform-����������
	GLuint getUniformLocation(std::string name);
private:
	// id "�������" ��������� ��������� ���������
	// ������������ ����� �������� �� ������������ ��������� ���������,
	// ���� ��� ��� ������������
	static GLuint currentProgram;

	// ��������� ��������� (������)
	GLuint program;

	// "����������" uniform-����������,
	// ������� ���������� ����� ����������� ������������
	// ����� ������ uniform-���������� � � �������� (location)
	map <string, GLuint> uniforms;
};