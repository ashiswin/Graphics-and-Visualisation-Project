#ifndef FBO_H
#define FBO_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

class FBO {
    public:
        FBO(int w, int h);
        void bind(); // bind FBO for rendering
        void unbind(); // unbind FBO
        void bindColorTexture(GLenum target); // Bind color texture to target texture location
        void bindDepthTexture(GLenum target); // bind depth texture to target texture location
        void unbindColorTexture();
        void unbindDepthTexture();
     
    private:
        GLuint fbo;
        GLuint colorTextureID, depthTextureID;
        int width, height;
        void enableColor(); // create a texture for the color attachment and attach it to FBO
        void enableDepth(); // create a texture for the depth attachment and attach it to FBO
};

#endif