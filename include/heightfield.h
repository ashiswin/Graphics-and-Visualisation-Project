#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <OpenGL/gl3.h>
#include <glm/mat4x4.hpp>

class FBO;
class Shader;
class Object;

class Heightfield {
    public:
        Heightfield(int width, int height);
        void draw(Shader* shader);
        void addHeight(float amount, glm::vec2 location);
        void stepSimulation();
        void calculateNormals();

    private:
        FBO *heightA; // for reading height data
        FBO *heightB; // for writing height data
        FBO *normals;

        Shader *addHeightShader;
        Shader *normalCalculationShader;
        Shader *stepSimulationShader;

        Object* o;

        GLuint vaoId;
        GLuint verticesVBO;
        GLuint indicesVBO;

        int width;
        int height;
        int nVertices;
        int nIndices;

        glm::mat4 modelMatrix;
        
        GLuint addHeightWidthLocation;
        GLuint addHeightHeightLocation;
        GLuint addHeightLocationLocation;
        GLuint addHeightAmountLocation;

        GLuint stepSimulationWidthLocation;
        GLuint stepSimulationHeightLocation;
        
        GLuint normalCalculationWidthLocation;
        GLuint normalCalculationHeightLocation;

        void generateGeometry();
        void prepareBuffers();
        void prepareShaders();
        void swapBuffers();
};

#endif