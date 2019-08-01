#include <GL/glew.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <object.h>
#include <texture.h>
#include <shader.h>
#include <camera.h>
#include <light.h>
#include <fbo.h>

Shader *shader;
Shader *simple, *simple_tex;

Camera *camera;
Light *light;
Texture *texture, *normals;
FBO *fbo;

glm::mat4 projectionMatrix;

float vertices[] = {
    -1, 1, 0,
    1, 1, 0,
    1, -1, 0,
    -1, -1, 0
};

float texcoords[] = {
    0, 0,
    1, 0,
    1, 1,
    0, 1
};

float vnormals[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
};

int indices[] = {
    0, 1, 3,
    1, 2, 3
};

void keyPressed(unsigned char c, int x, int y) {
    switch(c) {
        case 'w':
            camera->move(0, 0, -0.02f);
            break;
        case 'd':
            camera->move(0.02f, 0, 0);
            break;
        case 'a':
            camera->move(-0.02f, 0, 0);
            break;
        case 's':
            camera->move(0, 0, 0.02f);
            break;
        case 'v':
            square->spin();
            break;
    }
    
    glutPostRedisplay();
}
void update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
	glutCreateWindow( "OpenGL" );
    
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) printf("GLEW init failed");

     // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LEQUAL);
    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0);
    

    glutKeyboardFunc(&keyPressed);
    glutDisplayFunc(update);

    //Start GLUT main loop
	glutMainLoop();

    return 0;
}