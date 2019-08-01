#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <OpenGL/gl3.h>
#include <glm/mat4x4.hpp>

class FBO;
class Shader;

class Heightfield {
    public:
        Heightfield(int width, int height);
        void draw(Shader* shader);
        void addHeight(float amount, int x, int y);

    private:
        FBO *heightA; // for reading height data
        FBO *heightB; // for writing height data
        FBO *normals;

        GLuint vaoId;
        GLuint verticesVBO;
        GLuint indicesVBO;

        int width;
        int height;
        int nVertices;
        int nIndices;

        glm::mat4 modelMatrix;

        void generateGeometry();
        void prepareBuffers();
};

#endif