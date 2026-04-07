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
	// ���������� ������������� ������
	void setTextureId(int id);
	// �������� ��������� ���������
	vec4& getColor();
	const vec4& getColor() const;
	mat4& getModelMatrix();
	const mat4& getModelMatrix() const;
	int getMeshId() const;
	// ������ ID ������
	int getTextureId() const;
private:
	// ������������� ������������� ����
	int meshId;
	// ������������� ������
	int textureId;
	// ���� �������
	vec4 color;
	// ������� ������ (������ ������� � ����������)
	mat4 modelMatrix;
};