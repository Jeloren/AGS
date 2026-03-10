#pragma once
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

// ����� ��� ������ � ����������� ��������
class GraphicObject
{
public:
	// ����������� �� ���������
	GraphicObject();
	// ���������� ���� �������
	void setColor(const vec4& color);
	// ���������� ������� �������
	void setPosition(const vec3& position);
	// ���������� ���� �������� � �������� ������������ ��� OY �� ������� �������
	void setAngle(float degree);
	// ���������� ������������� ������������� ����
	void setMeshId(int id);
	// �������� ��������� ���������
	vec4& getColor();
	mat4& getModelMatrix();
	int getMeshId();
private:
	// ������������� ������������� ����
	int meshId;
	// ���� �������
	vec4 color;
	// ������� ������ (������ ������� � ����������)
	mat4 modelMatrix;
};