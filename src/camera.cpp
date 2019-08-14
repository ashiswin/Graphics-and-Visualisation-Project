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

void Camera::rotate(float a, float b, float c) {
    // pitch x, yaw y, roll z
    pitch = a + pitch;
    yaw = b + yaw;
    roll = c + roll;
}

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 view = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, -position);

    return view;
}

glm::vec3 Camera::getPosition() {

    return position;
}
