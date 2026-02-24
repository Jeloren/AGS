#include "Camera.h"
#include <algorithm>

Camera::Camera() : center(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f), 
                   phi(1.57f), theta(0.5f), radius(10.0f) {
    updateEye();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(eye, center, up);
}

glm::mat4 Camera::getProjectionMatrix(int width, int height) const {
    if (height == 0) height = 1;
    return glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void Camera::rotate(float dPhi, float dTheta) {
    phi += dPhi;
    theta += dTheta;
    // Ограничиваем theta, чтобы камера не перевернулась (от 0.1 до 1.5 рад)
    theta = std::clamp(theta, 0.1f, 1.5f);
    updateEye();
}

void Camera::zoom(float delta) {
    radius += delta;
    radius = std::clamp(radius, 2.0f, 30.0f);
    updateEye();
}

void Camera::move(float dx, float dz) {
    // Вектор направления взгляда в плоскости XZ
    float fwdX = -cos(phi);
    float fwdZ = -sin(phi);
    // Вектор "вправо"
    float rightX = sin(phi);
    float rightZ = -cos(phi);

    center.x += fwdX * dz + rightX * dx;
    center.z += fwdZ * dz + rightZ * dx;
    updateEye();
}

void Camera::updateEye() {
    // Сферические координаты в декартовы
    eye.x = center.x + radius * cos(theta) * cos(phi);
    eye.y = center.y + radius * sin(theta);
    eye.z = center.z + radius * cos(theta) * sin(phi);
}