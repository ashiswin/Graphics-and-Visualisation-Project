#ifndef TEXTURE_H
#define TEXTURE_H

#include <OpenGL/gl3.h>

class Texture {
    public:
        Texture();
        void loadFromFile(char* filename);
        void bind(GLenum target);
        void unbind(GLenum target);
    
    private:
        GLuint textureId;
};

#endif