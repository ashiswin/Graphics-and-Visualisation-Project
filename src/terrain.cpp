#include <iostream>
#include <vector>

#include <terrain.h>
#include <object.h>
#include <shader.h>

Terrain::Terrain(int detail) {
    this->detail = detail;
}

Object * Terrain::generateGeometry() {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned> indices;

    for(int i = 0; i < detail; i++) {
        for(int j = 0; j < detail; j++) {
            vertices.push_back(glm::vec3(i / (float) detail, 0, -j / (float) detail));

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

    std::cout << "Generated " << vertices.size() << " vertices and " << indices.size() << " indices for terrain." << std::endl;

    // Convert data into C arrays
    float *vertexArray = (float*) malloc(vertices.size() * 3 * sizeof(float));

    for(int i = 0; i < vertices.size(); i++) {
        vertexArray[i * 3] = vertices[i][0];
        vertexArray[i * 3 + 1] = vertices[i][1];
        vertexArray[i * 3 + 2] = vertices[i][2];
    }

    plane = new Object();
    plane->loadVertices(vertexArray, &indices[0], vertices.size(), indices.size());
    
    return plane;
}