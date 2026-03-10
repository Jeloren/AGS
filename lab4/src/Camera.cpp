#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;



Camera::Camera() : 
    position(vec3(5.0f, 5.0f, 5.0f)), // ��������� ������� ������
    target(vec3(0.0f, 0.0f, 0.0f)),    // �����, � ������� ������� ������
    up(vec3(0.0f, 1.0f, 0.0f)),        // ������ "�����"
    fovy(45.0f),                       // ���� ������
    aspect(4.0f / 3.0f),               // ����������� ������
    zNear(0.1f),                       // ������� ��������� ���������
    zFar(100.0f)                       // ������� ��������� ���������
{
    // ������������� ������
    recalculateViewMatrix();
    setProjectionMatrix(fovy, aspect, zNear, zFar);
}


void Camera::rotate(float horizAngle, float vertAngle) {
    vec3 direction = target - position;
    vec3 right = normalize(glm::cross(direction, up));

    // �������������� ��������
    mat4 horizontalRot = glm::rotate(mat4(1.0f), radians(-horizAngle), up);
    direction = vec3(horizontalRot * vec4(direction, 0.0f));

    // ������������ ��������
    mat4 verticalRot = glm::rotate(mat4(1.0f), radians(-vertAngle), right);
    direction = vec3(verticalRot * vec4(direction, 0.0f));

    // ����������� ������������� ����
    float angle = degrees(acos(dot(normalize(direction), up)));
    if (angle < 95.0f || angle > 175.0f) {
        return;
    }

    // ���������� ������� ������
    position = target - direction;
    recalculateViewMatrix();
}

void Camera::setProjectionMatrix(float fovy, float aspect, float zNear, float zFar) {
    projectionMatrix = perspective(radians(fovy), aspect, zNear, zFar);
}

mat4& Camera::getProjectionMatrix() {
    return projectionMatrix;
}

mat4& Camera::getViewMatrix() {
    return viewMatrix;
}

void Camera::zoom(float dR) {
    // ��������� ����������� ������
    vec3 direction = normalize(target - position);

    // �������� ������� ������
    position += direction * dR;

    recalculateViewMatrix();
}

void Camera::recalculateViewMatrix()
{
    viewMatrix = lookAt(position, target, up);
}

void Camera::moveOXZ(float dx, float dz)
{
    // ��������� ������ "������"
    vec3 right = normalize(cross(target - position, up));

    // ���������� ������ � ����� ����������
    position += right * dx + normalize(cross(up, right)) * dz;
    target += right * dx + normalize(cross(up, right)) * dz;

    recalculateViewMatrix();
}