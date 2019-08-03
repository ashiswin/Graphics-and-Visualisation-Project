#ifndef SHADER_H
#define SHADER_H

#include <OpenGL/gl3.h>
#include <glm/mat4x4.hpp>

class DirectionalLight;
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
        void loadLight(DirectionalLight *light);
        void enableTexture();
        void enableNormalMap();
        void attach();
        void detach();

        int getUniformLocation(const char* uniformName);

    private:

        GLuint program;

        int modelMatrixLocation;
        int projectionMatrixLocation;
        int viewMatrixLocation;

        int lightDirectionLocation;
        int lightColorLocation;
        int shineDamperLocation;
        int reflectivityLocation;
        int hasNormalMapLocation;
        int normalMapLocation;
        int textureLocation;
};

#endif