#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/mat4x4.hpp>

class Shader;
class Object;
class Texture;

class Skybox {
    public:
        Skybox();
        void setClippingPlane(glm::vec4 plane);
        void disableClippingPlane();
        void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
        
    private:
        Shader *shader;
        Object *skybox;
        Texture *texture;

        int clippingPlaneLocation;
        bool clippingPlaneEnabled;
        glm::vec4 clippingPlane;

        glm::mat4 modelMatrix;

        void prepareShaders();
        void prepareGeometry();
        void prepareTexture();
};

#endif