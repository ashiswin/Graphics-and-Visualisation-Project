#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif
#include <glm/mat4x4.hpp>

class FBO;
class Shader;
class Object;

class Heightfield {
    public:
        Heightfield(int detail, float scale);
        void draw(Shader* shader);
        void addHeight(float amount, glm::vec2 location);
        void lightShading(FBO *reflectFBO, FBO *refractFBO);
        void stepSimulation();
        void calculateNormals();
        void connectTexture();
        void bindNormalMap();
        void unbindNormalMap();

    private:
        FBO *heightA; // for reading height data
        FBO *heightB; // for writing height data
        FBO *normals;
        FBO *lightFBO;

        Shader *addHeightShader;
        Shader *normalCalculationShader;
        Shader *stepSimulationShader;
        Shader *lightTextureShader;

        Object* o;

        GLuint vaoId;
        GLuint verticesVBO;
        GLuint indicesVBO;

        int detail;
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

        GLuint reflectTextureLocation;
        GLuint refractTextureLocation;

        void generateGeometry();
        void prepareBuffers();
        void prepareShaders();
        void swapBuffers();
};

#endif