#ifndef LIGHTMESH_H
#define LIGHTMESH_H

class Object;
class Shader;
class FBO;

class LightMesh {
    public:
        LightMesh(int detail);
        FBO *draw(glm::mat4 projectionMatrix, Camera *camera, DirectionalLight *light);
    
    private:
        int detail;

        Object *plane;
        
        Shader *causticShader;

        FBO *caustics;

        void generateGeometry();
        void prepareBuffers();  
        void prepareShaders();
};
#endif