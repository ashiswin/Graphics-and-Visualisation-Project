#ifndef OBJECT_H
#define OBJECT_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif
#include <glm/mat4x4.hpp>

class Shader;
class Texture;

class Object {
    public:
        Object();
        void loadVertices(float* vertices, unsigned int* indices, int nVertices, int nIndices);
        void loadVertices(float* vertices, float* texcoords, float* normals, int* indices, int nVertices, int nIndices);
        void loadFromObj(char* filename);
        void loadFromImage(char* filename);
        void setShader(Shader* shader);
        Shader* getShader();
        void setTexture(Texture* texture);
        void setNormalMap(Texture* normals);
        void generateGeometry();

        void draw();
        void spin();
        void rotate(float pitch, float yaw, float roll);
        void scale(float x, float y, float z);
        void move(float dx, float dy, float dz);
        void loadSphere(float sectorCount, float stackCount, float radius);
        
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
        bool hasNormals = false;
        bool hasNormalMap = false;
        float reflectivity, shineDampener;
        Texture* texture;
        Texture* normalMap;

        glm::mat4 transformation;
};
#endif