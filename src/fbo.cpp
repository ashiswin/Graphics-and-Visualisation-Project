#include <iostream>
#include <cstdlib>

#include <fbo.h>

FBO::FBO(int w, int h) {
    glGenFramebuffers(1, &fbo);

    width = w;
    height = h;

    enableColor(); 
    // enableDepth();
}

void FBO::enableColor() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &colorTextureID);
    glBindTexture(GL_TEXTURE_2D, colorTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // std::cout << "Color buffer: " << colorTextureID << std::endl;

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0);
    
    // GLenum err;
    // while((err = glGetError()) != GL_NO_ERROR)
    // {
    //     std::cout << "Error in Object::enableColor 1: " << err << std::endl;
    // }
    glClear(GL_COLOR_BUFFER_BIT);
    unbind();
}

void FBO::enableDepth() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &depthTextureID);
    glBindTexture(GL_TEXTURE_2D, depthTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT32, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::enableDepth 1: " << err << std::endl;
    }

    unbind();
}

void FBO::bindColorTexture(GLenum target) {

    glActiveTexture(target);
    glBindTexture(GL_TEXTURE_2D, colorTextureID);

    // std::cout << colorTextureID << std::endl;

}

void FBO::unbindColorTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::unbindDepthTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}


void FBO::bindDepthTexture(GLenum target) {

    glActiveTexture(target);
    glBindTexture(GL_TEXTURE_2D, depthTextureID);

}

void FBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(result != GL_FRAMEBUFFER_COMPLETE) {
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! " << result << std::endl;
    }
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::bind 1: " << err << std::endl;
    }
}

void FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}