#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

class Texture {
    public:
        Texture();
        void loadFromFile(char* filename);
        void bind(GLenum target);
        void unbind();
    
    private:
        GLuint textureId;
};

#endif