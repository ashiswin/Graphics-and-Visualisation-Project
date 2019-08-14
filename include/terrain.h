#ifndef TERRAIN_H
#define TERRAIN_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif
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