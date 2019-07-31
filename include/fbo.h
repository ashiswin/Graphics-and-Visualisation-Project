#ifndef FBO_H
#define FBO_H

#include <OpenGL/gl3.h>

class FBO {
    public:
        FBO(int w, int h);
        void enableColor(); // create a texture for the color attachment and attach it to FBO
        void enableDepth(); // create a texture for the depth attachment and attach it to FBO
        void bind(); // bind FBO for rendering
        void unbind(); // unbind FBO
        void bindColorTexture(GLenum target); // Bind color texture to target texture location
        void bindDepthTexture(GLenum target); // bind depth texture to target texture location
     
    private:
        GLuint fbo;
        GLuint colorTextureID, depthTextureID;
        int width, height;
};

#endif