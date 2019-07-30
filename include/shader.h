#ifndef SHADER_H
#define SHADER_H

#include <OpenGL/gl3.h>
#include <glm/mat4x4.hpp>

class Light;
class Shader {
    public:
        Shader();
        void attachShader(GLenum shaderType, const char* filename);
        bool compile();
        void bindAttribute(int attrib, const char* variableName);
        void loadModelMatrix(glm::mat4 matrix);
        void loadProjectionMatrix(glm::mat4 matrix);
        void loadViewMatrix(glm::mat4 matrix);
        void loadSpecularComponents(float shineDamper, float reflectivity);
        void loadLight(Light *light);
        void attach();
        void detach();

    private:
        int getUniformLocation(const char* uniformName);

        GLuint program;

        int modelMatrixLocation;
        int projectionMatrixLocation;
        int viewMatrixLocation;

        int lightPositionLocation;
        int lightColorLocation;
        int shineDamperLocation;
        int reflectivityLocation;
};

#endif