#include <iostream>

#include <skybox.h>
#include <shader.h>
#include <object.h>
#include <texture.h>

Skybox::Skybox() {
    modelMatrix = glm::mat4(1.0f);
    clippingPlaneEnabled = false;

    prepareGeometry();
    prepareShaders();
    prepareTexture();
}

void Skybox::prepareGeometry() {
    skybox = new Object();
    skybox->loadFromObj("assets/sphere.obj");
    skybox->scale(200);
}

void Skybox::prepareShaders() {
    shader = new Shader();
    std::cout << "Compiling skybox vertex shader..." << std::endl;
    shader->attachShader(GL_VERTEX_SHADER, "shaders/skybox_vertex.glsl");
    std::cout << "Compiling skybox fragment shader..." << std::endl;
    shader->attachShader(GL_FRAGMENT_SHADER, "shaders/skybox_fragment.glsl");
    if(!shader->compile()) std::cout << "Error compiling skybox shader!" << std::endl;

    clippingPlaneLocation = shader->getUniformLocation("clippingPlane");
}

void Skybox::prepareTexture() {
    texture = new Texture();
    texture->loadFromFile("assets/textures/env.jpg");
}

void Skybox::setClippingPlane(glm::vec4 clippingPlane) {
    this->clippingPlane = glm::vec4(clippingPlane);
    clippingPlaneEnabled = true;
}

void Skybox::disableClippingPlane() {
    clippingPlaneEnabled = false;
}
void Skybox::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    shader->attach();
    
    shader->loadProjectionMatrix(projectionMatrix);
    shader->loadViewMatrix(viewMatrix);
    shader->enableTexture();

    texture->bind(GL_TEXTURE0);
    
    if(clippingPlaneEnabled) {
        glEnable(GL_CLIP_DISTANCE0);
        glUniform4fv(clippingPlaneLocation, 1, &clippingPlane[0]);
    }

    skybox->setShader(shader);
    skybox->draw();

    if(clippingPlaneEnabled) {
        glDisable(GL_CLIP_DISTANCE0);
    }

    texture->unbind();
    shader->detach();
}