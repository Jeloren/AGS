#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// ����� ��� ������ � �������
class Camera
{
public:
	// ����������� �� ���������
	Camera();
	// ���������� ������� ��������
	void setProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	// �������� ������� ��������
	mat4& getProjectionMatrix();
	// �������� ������� ����
	mat4& getViewMatrix();
	// ����������� ������ � ����� ���������� � �������������� ��������� (OXZ)
	void moveOXZ(float dx, float dz);
	// ��������� � �������������� � ������������ ��������� (���� �������� � ��������)
	void rotate(float horizAngle, float vertAngle);
	// ����������/������� ������ �/�� ����� ����������
	void zoom(float dR);
private:
	// ����������� ������� ����
	void recalculateViewMatrix();
private:
	mat4 viewMatrix; // ������� ����
	mat4 projectionMatrix; // ������� ��������
	vec3 position;
	vec3 target;
	vec3 up;
	vec3 right;
	vec3 worldUp;
	float fovy;  // ���� ������ (� ��������)
	float aspect; // ����������� ������ (������ / ������)
	float zNear; // ������� ��������� ���������
	float zFar;  // ������� ��������� ���������
};