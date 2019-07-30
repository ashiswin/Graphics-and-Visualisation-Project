#include <iostream>
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera() {
    position = glm::vec3(0, 0, 0);
    pitch = yaw = roll = 0.0f;
}

void Camera::move(float dx, float dy, float dz) {
    position.x += dx;
    position.y += dy;
    position.z += dz;
}

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 view = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, -position);

    return view;
}