#ifndef LIGHTMESH_H
#define LIGHTMESH_H

class Object;
class Shader;

class LightMesh {
    public:
        LightMesh(int detail);
    
    private:
        Object *plane;
        
        Shader *causticShader;

        void generateGeometry(int detail);
        void prepareShaders();
};
#endif