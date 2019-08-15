#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/vec3.hpp>
// #include <glm/vec3.hpp>
#include <glm/vec2.hpp>

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
    yaw += 10.0f;

    transformation = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Object::rotate(float pitch, float yaw, float roll) {
    transformation = glm::rotate(transformation, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    transformation = glm::rotate(transformation, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Object::scale(float x, float y, float z) {
    transformation = glm::scale(transformation, glm::vec3(x, x, z));
}

void Object::move(float dx, float dy, float dz) {
    transformation = glm::translate(transformation, glm::vec3(dx, dy, dz));
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
            int spaces = 0;
            int slashes = 0;

            for(int i = 0; i < strlen(buffer); i++) {
                if(buffer[i] == ' ') spaces++;
                if(buffer[i] == '/') slashes++;
            }
            // std::cout << "COUNTING" << std::endl;
            // std::cout << spaces << slashes << std::endl;
            if(spaces == 4 && slashes == 4) {
                ss >> v1 >> scratch >> t1;
                ss >> v2 >> scratch >> t2;
                ss >> v3 >> scratch >> t3;
                ss >> v4 >> scratch >> t4;

                n1 = n2 = n3 = n4 = -1;
            } else if (spaces == 3 && slashes == 6) {
                ss >> v1 >> scratch >> t1 >> scratch >> n1;
                ss >> v2 >> scratch >> t2 >> scratch >> n2;
                ss >> v3 >> scratch >> t3 >> scratch >> n3;
            } else {
                ss >> v1 >> scratch >> t1;
                ss >> v2 >> scratch >> t2;
                ss >> v3 >> scratch >> t3;

                n1 = n2 = n3 = -1;
            }

            std::vector<unsigned> face;

            if(slashes == 6) {
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

            // std::cout << vertexPointer << " " << vecv.size() << std::endl;

            glm::vec3 currentVert = vecv[vertexPointer];
            vertices[vertexPointer * 3] = currentVert[0];
            vertices[vertexPointer * 3 + 1] = currentVert[1];
            vertices[vertexPointer * 3 + 2] = currentVert[2];

            // std::cout << vertexPointer << " " << vecv.size() << std::endl;

            if(vect.size() > 0) {
                glm::vec2 currentTex = vect[v[6 + i] - 1];
                texcoords[vertexPointer * 2] = currentTex[0];
                texcoords[vertexPointer * 2 + 1] = currentTex[1];   
            }

            // std::cout << vertexPointer << " " << vect.size() << std::endl;

            glm::vec3 currentNorm;
            // std::cout << v[3+i] << std::endl;
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
    
    hasNormals = true;
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
    GLenum err;
    shader->loadModelMatrix(transformation);
    shader->loadSpecularComponents(shineDampener, reflectivity);

    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::draw 0: " << err << std::endl;
    }
    glBindVertexArray(vaoId);
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::draw 1: " << err << std::endl;
    }
    glEnableVertexAttribArray(0);
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::draw 2: " << err << std::endl;
    }
    if(hasTextures) glEnableVertexAttribArray(1);
    if(hasNormals) glEnableVertexAttribArray(2);

    // GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::draw 3: " << err << std::endl;
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
        std::cout << "Error in Object::draw 4: " << err << std::endl;
    }

    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::draw 5: " << err << " ----- " << nIndices << std::endl;
    }

    glDisableVertexAttribArray(2);
    if(hasTextures) glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Object::loadSphere(float sectorCount, float stackCount, float radius) {
    // clear memory of prev arrays
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius; 
    // std::cout << lengthInv << std::endl;   // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texcoords.push_back(s);
            texcoords.push_back(t);
        }
    }

    std::vector<int> indices;
    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    std::cout << normals[0] << " " << normals[1] << " " << normals[2]<< std::endl;
    hasNormals = true;
    hasTextures = true;
    loadVertices(&vertices[0], &texcoords[0], &normals[0], &indices[0], vertices.size(), indices.size());
}
