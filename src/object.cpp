#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>

#include <object.h>
#include <shader.h>
#include <texture.h>

Object::Object() {
    glGenVertexArrays(1, &vaoId);

    hasNormalMap = false;

    transformation = glm::mat4(1.0f);
    yaw = 0.0f;

    shineDampener = 1;
    reflectivity = 0;
}

void Object::spin() {
    yaw += 1.0f;

    transformation = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Object::loadVertices(float* vertices, unsigned int* indices, int nVertices, int nIndices) {
    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vaoId);

    glGenBuffers(1, &indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->nIndices = nIndices;
}

void Object::loadVertices(float* vertices, float* texcoords, float* normals, int* indices, int nVertices, int nIndices) {
    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &texcoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->texcoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices * 2 * sizeof(float), texcoords, GL_STATIC_DRAW);

    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), normals, GL_STATIC_DRAW);

    glBindVertexArray(vaoId);

    glGenBuffers(1, &indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->texcoordsVBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->nIndices = nIndices;
    this->nVertices = nVertices;
    hasTextures = true;
}

void Object::loadFromObj(char* filename) {
    std::vector<glm::vec3> vecv;
    std::vector<glm::vec2> vect;
    std::vector<glm::vec3> vecn;
    std::vector< std::vector<unsigned> > vecf;

    std::ifstream input(filename);

    char buffer[1024];

    while(input.getline(buffer, 1024)) {
        std::stringstream ss(buffer);
        std::string s;

        ss >> s;

        if(s == "v") {
            glm::vec3 v;

            ss >> v[0] >> v[1] >> v[2];
            vecv.push_back(v);
        }
        else if(s == "vn") {
            glm::vec3 n;

            ss >> n[0] >> n[1] >> n[2];
            vecn.push_back(n);
        }
        else if(s == "vt") {
            glm::vec2 t;

            ss >> t[0] >> t[1];
            vect.push_back(t);
        }
        else if(s == "f") {
            char scratch;
            int v1, v2, v3, n1, n2, n3, t1, t2, t3;
            int v4, n4, t4;
            int slashes = 0;

            for(int i = 0; i < strlen(buffer); i++) {
                if(buffer[i] == '/') slashes++;
            }
            
            if(slashes > 3) {
                ss >> v1 >> scratch >> t1 >> scratch >> n1;
                ss >> v2 >> scratch >> t2 >> scratch >> n2;
                ss >> v3 >> scratch >> t3 >> scratch >> n3;

                if(slashes > 6) ss >> v4 >> scratch >> t4 >> scratch >> n4;
            }
            else {
                ss >> v1 >> scratch >> t1;
                ss >> v2 >> scratch >> t2;
                ss >> v3 >> scratch >> t3;

                n1 = n2 = n3 = -1;
            }

            std::vector<unsigned> face;

            if(slashes <= 6) {
                face.push_back(v1);
                face.push_back(v2);
                face.push_back(v3);
                face.push_back(n1);
                face.push_back(n2);
                face.push_back(n3);
                face.push_back(t1);
                face.push_back(t2);
                face.push_back(t3);

                vecf.push_back(face);
            }
            else {
                face.push_back(v1);
                face.push_back(v2);
                face.push_back(v4);
                face.push_back(n1);
                face.push_back(n2);
                face.push_back(n4);
                face.push_back(t1);
                face.push_back(t2);
                face.push_back(t4);
                vecf.push_back(face);

                std::vector<unsigned> face2;
                face2.push_back(v2);
                face2.push_back(v3);
                face2.push_back(v4);
                face2.push_back(n2);
                face2.push_back(n3);
                face2.push_back(n4);
                face2.push_back(t2);
                face2.push_back(t3);
                face2.push_back(t4);

                vecf.push_back(face2);
            }
        }
    }
    
    float *vertices = (float*) malloc(sizeof(float) * vecv.size() * 3);
    float *normals = (float*) malloc(sizeof(float) * vecv.size() * 3);
    float *texcoords = (float*) malloc(sizeof(float) * vecv.size() * 2);
    std::vector<unsigned> indices;

    if(vertices == NULL) std::cout << "FAILED TO ALLOC VERTS" << std::endl;
    if(normals == NULL) std::cout << "FAILED TO ALLOC NORMALS" << std::endl;
    if(texcoords == NULL) std::cout << "FAILED TO ALLOC TEXCOORDS" << std::endl;
    
    for(std::vector<unsigned> v : vecf) {
        for(int i = 0; i < 3; i++) {
            int vertexPointer = v[i] - 1;
            indices.push_back(vertexPointer);

            glm::vec3 currentVert = vecv[vertexPointer];
            vertices[vertexPointer * 3] = currentVert[0];
            vertices[vertexPointer * 3 + 1] = currentVert[1];
            vertices[vertexPointer * 3 + 2] = currentVert[2];

            if(vect.size() > 0) {
                glm::vec2 currentTex = vect[v[6 + i] - 1];
                texcoords[vertexPointer * 2] = currentTex[0];
                texcoords[vertexPointer * 2 + 1] = currentTex[1];   
            }

            glm::vec3 currentNorm;
            if(v[3 + i] != -1) {
                currentNorm = vecn[v[3 + i] - 1];
            }
            else {
                currentNorm = glm::normalize(-glm::cross(vecv[v[1] - 1] - vecv[v[0] - 1], vecv[v[2] - 1] - vecv[v[0] - 1]));
            }
            normals[vertexPointer * 3] = currentNorm[0];
            normals[vertexPointer * 3 + 1] = currentNorm[1];
            normals[vertexPointer * 3 + 2] = currentNorm[2];
        }
    }
    
    nIndices = indices.size();
    nVertices = vecv.size();
    
    glGenBuffers(1, &verticesVBO);
    glGenBuffers(1, &normalsVBO);
    if(vect.size() > 0) {
        glGenBuffers(1, &texcoordsVBO);
        hasTextures = true;
    }
    glGenBuffers(1, &indicesVBO);

    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, vecv.size() * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, vecv.size() * 3 * sizeof(float), normals, GL_STATIC_DRAW);

    if(hasTextures) {
        glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
        glBufferData(GL_ARRAY_BUFFER, vecv.size() * 2 * sizeof(float), texcoords, GL_STATIC_DRAW);
    }

    glBindVertexArray(vaoId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    
    if(hasTextures) {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
    }

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    free(vertices);
    free(normals);
    free(texcoords);

    std::cout << "Loaded " << nIndices / 3 << " faces from " << filename << std::endl;
}

void Object::setShader(Shader* shader) {
    this->shader = shader;
}

Shader* Object::getShader() {
    return shader;
}

void Object::setTexture(Texture* texture) {
    this->texture = texture;
}

void Object::setNormalMap(Texture* normals) {
    this->normalMap = normals;
    hasNormalMap = true;
}

void Object::draw() {
    shader->loadModelMatrix(transformation);
    shader->loadSpecularComponents(shineDampener, reflectivity);
    glBindVertexArray(vaoId);
    glEnableVertexAttribArray(0);
    if(hasTextures) glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::draw 0: " << err << std::endl;
    }
    // if(texture != NULL) {
    //     shader->enableTexture();
    //     texture->bind(GL_TEXTURE0);
    // }
    // if(hasNormalMap) {
    //     shader->enableNormalMap();
    //     normalMap->bind(GL_TEXTURE1);
    // }
    
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::draw 1: " << err << std::endl;
    }

    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::draw 2: " << err << std::endl;
    }

    glDisableVertexAttribArray(2);
    if(hasTextures) glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}