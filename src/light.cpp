#include "light.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color) {
    this->position = position;
    this->color = color;
}

glm::vec3 PointLight::getPosition() {
    return position;
}

glm::vec3 PointLight::getColor() {
    return color;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color) {
    this->direction = direction;
    this->color = color;
}

glm::vec3 DirectionalLight::getDirection() {
    return direction;
}

glm::vec3 DirectionalLight::getColor() {
    return color;
}