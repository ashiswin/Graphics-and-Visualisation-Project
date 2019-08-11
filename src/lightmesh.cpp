#include <GL/glew.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <object.h>
#include <texture.h>
#include <shader.h>
#include <camera.h>
#include <terrain.h>
#include <vector>
#include <light.h>
#include <fbo.h>
#include <heightfield.h>
#include <lightmesh.h>
#include <skybox.h>

LightMesh::LightMesh(int detail) {
    this->detail = detail;
    
    generateGeometry();
    prepareBuffers();
    prepareShaders();
}

void LightMesh::generateGeometry() {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned> indices;

    for(int i = 0; i < detail; i++) {
        for(int j = 0; j < detail; j++) {
            vertices.push_back(glm::vec3(i / (float) detail, j / (float) detail, 0));

            if(i == 0 || j == 0) continue;

            // Top left triangle
            indices.push_back(((i - 1) * detail) + j - 1);
            indices.push_back(((i - 1) * detail) + j);
            indices.push_back((i * detail) + j - 1);

            // Bottom right triangle
            indices.push_back(((i - 1) * detail) + j);
            indices.push_back((i * detail) + j);
            indices.push_back((i * detail) + j - 1);
        }
    }

    std::cout << "Generated " << vertices.size() << " vertices and " << indices.size() << " indices for lightmesh." << std::endl;

    // Convert data into C arrays
    float *vertexArray = (float*) malloc(vertices.size() * 3 * sizeof(float));

    for(int i = 0; i < vertices.size(); i++) {
        vertexArray[i * 3] = vertices[i][0];
        vertexArray[i * 3 + 1] = vertices[i][1];
        vertexArray[i * 3 + 2] = vertices[i][2];
    }

    plane = new Object();
    plane->loadVertices(vertexArray, &indices[0], vertices.size(), indices.size());
}

void LightMesh::prepareBuffers() {
    caustics = new FBO(detail, detail);
}
void LightMesh::prepareShaders() {
    causticShader = new Shader();
    std::cout << "Compiling caustic vertex shader..." << std::endl;
    causticShader->attachShader(GL_VERTEX_SHADER, "shaders/caustics_vertex.glsl");
    std::cout << "Compiling caustic fragment shader..." << std::endl;
    causticShader->attachShader(GL_FRAGMENT_SHADER, "shaders/caustics_fragment.glsl");
    if(!causticShader->compile()) std::cout << "Error compiling caustic shader!" << std::endl;
}

FBO *LightMesh::draw(glm::mat4 projectionMatrix, Camera *camera, DirectionalLight *light) {

    caustics->bind();
    glClear(GL_COLOR_BUFFER_BIT);
    
    causticShader->attach();
    // causticShader->loadProjectionMatrix(projectionMatrix);
    // causticShader->loadViewMatrix(camera->getViewMatrix());
    causticShader->loadLight(light);
    causticShader->enableTexture();

    plane->setShader(causticShader);
    plane->draw();

    float pixels[200 * 200 * 3];
    glReadPixels(0, 0, 200, 200, GL_RGB, GL_FLOAT, pixels);
    for(int i = 0; i < 200; i++) {
        for(int j = 0; j < 200; j++) {
            std::cout << "(";
            for(int k = 0; k < 3; k++) {
                std::cout << pixels[(i * (200 * 3)) + (j * 3) + k] << ",";
            }
            std::cout << ") ";
        }
        std::cout << std::endl;
    }

    caustics->unbind();
    causticShader->detach();

    return caustics;
}