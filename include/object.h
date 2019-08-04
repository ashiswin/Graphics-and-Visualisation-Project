#ifndef OBJECT_H
#define OBJECT_H

#include <OpenGL/gl3.h>
#include <glm/mat4x4.hpp>

class Shader;
class Texture;

class Object {
    public:
        Object();
        void loadVertices(float* vertices, unsigned int* indices, int nVertices, int nIndices);
        void loadVertices(float* vertices, float* texcoords, float* normals, int* indices, int nVertices, int nIndices);
        void loadFromObj(char* filename);
        void setShader(Shader* shader);
        Shader* getShader();
        void setTexture(Texture* texture);
        void setNormalMap(Texture* normals);

        void draw();
        void spin();
        void scale(int val);
        
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
        bool hasNormalMap = false;
        float reflectivity, shineDampener;
        Texture* texture;
        Texture* normalMap;

        glm::mat4 transformation;
};
#endif