#ifndef LIGHTMESH_H
#define LIGHTMESH_H

class Object;
class Shader;
class FBO;

class LightMesh {
    public:
        LightMesh(int detail);
    
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