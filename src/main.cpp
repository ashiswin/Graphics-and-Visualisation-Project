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

Object *objTest;
Shader *shader;
Camera *camera;
Light *light;
Texture *texture, *normals;

glm::mat4 projectionMatrix;

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
            objTest->spin();
            break;
    }
    
    glutPostRedisplay();
}
void update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Shader* shader = objTest->getShader();
    shader->attach();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->loadViewMatrix(camera->getViewMatrix());
    shader->loadLight(light);

    objTest->draw();

    shader->detach();

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
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LEQUAL);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0);
    GLint iMultiSample = 0;
    GLint iNumSamples = 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    glGetIntegerv(GL_SAMPLES, &iNumSamples);
    printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);

    projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.f);

    shader = new Shader();
    shader->attachShader(GL_VERTEX_SHADER, "shaders/basic_vertex.glsl");
    shader->attachShader(GL_FRAGMENT_SHADER, "shaders/basic_fragment.glsl");
    if(!shader->compile()) printf("Error compiling shader\n");
    
    light = new Light(glm::vec3(0, 20, 10.0), glm::vec3(1.0, 1.0, 1.0));
    
    camera = new Camera();
    camera->move(0, 1, 5);
    
    texture = new Texture();
    // texture->loadFromFile("assets/textures/panana_tree_diffuse.jpg");
    texture->loadFromFile("assets/textures/tree.png");

    objTest = new Object();
    objTest->loadFromObj("assets/tree.obj");
    objTest->setShader(shader);
    objTest->setTexture(texture);

    glutKeyboardFunc(&keyPressed);
    glutDisplayFunc(update);

    //Start GLUT main loop
	glutMainLoop();

    return 0;
}