#ifndef TERRAIN_H
#define TERRAIN_H

#include <OpenGL/gl3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class FBO;
class Shader;
class Object;

class Terrain {
    public:
        Terrain(int detail);
        void draw(Shader* shader);
        Object * generateGeometry();

    private:
        int detail;

        Object *plane;

};
#endif