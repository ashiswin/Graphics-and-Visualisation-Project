#include <iostream>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <heightfield.h>
#include <fbo.h>
#include <shader.h>

Heightfield::Heightfield(int width, int height) {
    this->width = width;
    this->height = height;
    this->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-width / 2, 0, 0));

    prepareBuffers();
    generateGeometry();
}

void Heightfield::prepareBuffers() {
    glGenBuffers(1, &verticesVBO);
    glGenBuffers(1, &indicesVBO);
    glGenVertexArrays(1, &vaoId);

    heightA = new FBO(width, height);
    heightB = new FBO(width, height);
    normals = new FBO(width, height);
}

void Heightfield::generateGeometry() {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned> indices;

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            vertices.push_back(glm::vec3(i, 0, -j));

            if(i == 0 || j == 0) continue;

            // Top left triangle
            indices.push_back(((i - 1) * width) + j - 1);
            indices.push_back(((i - 1) * width) + j);
            indices.push_back((i * width) + j - 1);

            // Bottom right triangle
            indices.push_back(((i - 1) * width) + j);
            indices.push_back((i * width) + j);
            indices.push_back((i * width) + j - 1);
        }
    }

    std::cout << "Generated " << vertices.size() << " vertices and " << indices.size() << " indices for heightmap." << std::endl;

    this->nVertices = vertices.size();
    this->nIndices = indices.size();

    // Convert data into C arrays
    float *vertexArray = (float*) malloc(vertices.size() * 3 * sizeof(float));
    // unsigned int *indexArray = (unsigned int*) malloc(indices.size() * sizeof(unsigned int));

    for(int i = 0; i < vertices.size(); i++) {
        vertexArray[i * 3] = vertices[i][0];
        vertexArray[i * 3 + 1] = vertices[i][1];
        vertexArray[i * 3 + 2] = vertices[i][2];
    }

    // Set up data in VBOs
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertexArray, GL_STATIC_DRAW);

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Heightfield::draw(Shader *shader) {
    shader->loadModelMatrix(modelMatrix);

    glBindVertexArray(vaoId);
    glEnableVertexAttribArray(0);

    
    glDrawElements(GL_TRIANGLES, this->nIndices, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}