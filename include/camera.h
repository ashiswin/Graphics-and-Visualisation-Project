#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera {
    public:
        Camera();
        void move(float dx, float dy, float dz);
        glm::mat4 getViewMatrix();
        void rotate(float a, float b, float c);

        glm::vec3 position;
        float pitch;
        float yaw;
        float roll;
};

#endif