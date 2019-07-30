#ifndef OBJECT_H
#define OBJECT_H

#include <OpenGL/gl3.h>
#include <glm/mat4x4.hpp>

class Shader;

class Object {
    public:
        Object();
        void loadVertices(float* vertices, int* indices, int nVertices, int nIndices);
        void loadFromObj(char* filename);
        void setShader(Shader* shader);
        Shader* getShader();
        void draw();
        void spin();
        
    private:
        GLuint verticesVBO;
        GLuint normalsVBO;
        GLuint texcoordsVBO;
        GLuint indicesVBO;
        GLuint vaoId;
        Shader* shader;

        int nIndices;
        int nVertices;

        float yaw;

        bool hasTextures = false;
        float reflectivity, shineDampener;
        
        glm::mat4 transformation;
};
#endif