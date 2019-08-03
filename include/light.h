#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>

class PointLight {
    public:
        PointLight(glm::vec3 position, glm::vec3 color);
        glm::vec3 getPosition();
        glm::vec3 getColor();
        
    private:
        glm::vec3 position;
        glm::vec3 color;
};

class DirectionalLight {
    public:
        DirectionalLight(glm::vec3 direction, glm::vec3 color);
        glm::vec3 getDirection();
        glm::vec3 getColor();
    
    private:
        glm::vec3 direction;
        glm::vec3 color;
};

#endif