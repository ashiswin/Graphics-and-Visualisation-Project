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
        void addHeight(float amount, glm::vec2 location);
        void stepSimulation();

    private:
        FBO *heightA; // for reading height data
        FBO *heightB; // for writing height data
        FBO *normals;

        Shader *addHeightShader;
        Shader *normalCalculationShader;
        Shader *stepSimulationShader;

        GLuint vaoId;
        GLuint verticesVBO;
        GLuint indicesVBO;

        int width;
        int height;
        int nVertices;
        int nIndices;

        glm::mat4 modelMatrix;

        
        GLuint program;

        GLuint widthLocation;
        GLuint heightLocation;
        GLuint locationLocation;
        GLuint amountLocation;
        
        void generateGeometry();
        void prepareBuffers();
        void prepareShaders();
};

#endif