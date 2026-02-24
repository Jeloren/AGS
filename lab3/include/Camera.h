#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(int width, int height) const;

    void rotate(float dPhi, float dTheta);
    void zoom(float delta);
    void move(float dx, float dz);

private:
    void updateEye();

    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;

    float phi;
    float theta;
    float radius;
};