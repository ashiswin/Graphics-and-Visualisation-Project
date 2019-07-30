#include <iostream>

#include <texture.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {
    glGenTextures(1, &textureId);
}

void Texture::loadFromFile(char* filename) {
    int width, height, n;

    float* data = stbi_loadf(filename, &width, &height, &n, 0);

    std::cout << "Loaded texture " << filename << " with size " << width << "x" << height << std::endl;

    glBindTexture(GL_TEXTURE_2D, textureId);
    if(n == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);
    if(n == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(GLenum target) {
    glActiveTexture(target);
    glBindTexture(GL_TEXTURE_2D, textureId);
}