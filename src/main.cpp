#include <GL/glew.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <object.h>
#include <texture.h>
#include <shader.h>
#include <camera.h>
#include <light.h>
#include <fbo.h>
#include <heightfield.h>
#include <lightmesh.h>

#define WIDTH 256
#define HEIGHT 256

// Shader *shader;
// Shader *simple, *simple_tex;

Shader *firstPass;
Shader *secondPass;
Shader *waterShader;

GLuint widthLocation, heightLocation;
GLuint reflectionLocation, refractionLocation;
Object *quad;
Object *skybox;

Camera *camera, *reflectCamera, *refractCamera;
DirectionalLight *light;
// Texture *texture, *normals;

FBO *fbo, *reflectFBO, *refractFBO;
Heightfield *water;
LightMesh *lightMesh;

glm::mat4 projectionMatrix;

Texture *skyboxTexture;

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

float normals[] = {
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
            camera->move(0, 0, -1);
            break;
        case 'd':
            camera->move(1, 0, 0);
            break;
        case 'a':
            camera->move(-1, 0, 0);
            break;
        case 's':
            camera->move(0, 0, 1);
            break;
        case 'v':
            // water->addHeight(10.0, glm::vec2(WIDTH / 2, HEIGHT / 2));
            water->addHeight(rand() % 20, glm::vec2(rand() % WIDTH, rand() % HEIGHT));
            break;
        case 'c':
            water->stepSimulation();
            water->calculateNormals();
            break;
    }
    
    glutPostRedisplay();
}

void update() {
    // Begin first pass of rendering (render to FBO)
    std::cout << "Begin first pass" << std::endl;
    // TODO: Bind FBO for drawing
    fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    firstPass->attach();
    firstPass->loadProjectionMatrix(projectionMatrix);
    firstPass->loadViewMatrix(camera->getViewMatrix());

    quad->setShader(firstPass);
    quad->draw();

    firstPass->detach();

    // TODO: Unbind FBO
    fbo->unbind();

    // Begin second pass of rendering (render to screen)
    std::cout << "Begin second pass" << std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    secondPass->attach();
    secondPass->loadProjectionMatrix(projectionMatrix);
    secondPass->loadViewMatrix(camera->getViewMatrix());
    secondPass->enableTexture();

    // TODO: Bind FBO color texture to GL_TEXTURE0
    quad->setShader(secondPass);
    quad->draw();

    // TODO: Unbind FBO color texture
    secondPass->detach();

    glutSwapBuffers();
}

void testHeightfield() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    secondPass->attach();
    secondPass->loadProjectionMatrix(projectionMatrix);
    secondPass->loadViewMatrix(camera->getViewMatrix());

    secondPass->enableTexture();
    skyboxTexture->bind(GL_TEXTURE0);

    skybox->draw();
    secondPass->detach();
    
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::thf: " << err << std::endl;
    }
    secondPass->attach();
    secondPass->loadProjectionMatrix(projectionMatrix);
    secondPass->loadViewMatrix(reflectCamera->getViewMatrix());


    reflectFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skybox->setShader(secondPass);
    skybox->draw();
    reflectFBO->unbind();

    secondPass->attach();
    secondPass->loadProjectionMatrix(projectionMatrix);
    secondPass->loadViewMatrix(refractCamera->getViewMatrix());

    refractFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skybox->setShader(secondPass);
    skybox->draw();
    refractFBO->unbind();

    waterShader->attach();
    waterShader->loadProjectionMatrix(projectionMatrix);
    waterShader->loadViewMatrix(camera->getViewMatrix());
    waterShader->loadLight(light);

    glUniform1f(widthLocation, WIDTH);
    glUniform1f(heightLocation, HEIGHT);
    glUniform1i(reflectionLocation, 2);
    glUniform1i(refractionLocation, 3);

    reflectFBO->bindColorTexture(GL_TEXTURE2);
    refractFBO->bindColorTexture(GL_TEXTURE3);

    water->draw(waterShader);

    reflectFBO->unbindColorTexture();
    refractFBO->unbindColorTexture();
    
    waterShader->detach();

    glutSwapBuffers();
}

void timer(int value) {
    water->stepSimulation();
    water->calculateNormals();
    
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

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
    
    camera = new Camera();
    camera->move(0, 2, 5);

    reflectCamera = new Camera();
    reflectCamera->move(0, 10, 5);
    reflectCamera->rotate(0, 90, 0);
    refractCamera = new Camera();
    refractCamera->move(0, -10, -5);
    refractCamera->rotate(0, -90, 0);

    light = new DirectionalLight(glm::vec3(1, -1, 0), glm::vec3(0, 0, 1));
    
    projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);

    water = new Heightfield(WIDTH, HEIGHT);
    
    lightMesh = new LightMesh(200);
    
    firstPass = new Shader();
    firstPass->attachShader(GL_VERTEX_SHADER, "shaders/simple_vertex.glsl");
    firstPass->attachShader(GL_FRAGMENT_SHADER, "shaders/simple_fragment.glsl");
    if(!firstPass->compile()) std::cout << "Error compiling first pass shader!" << std::endl;
    // firstPass->enableTexture();
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::main: " << err << std::endl;
    }
    waterShader = new Shader();
    std::cout << "Compiling water vertex shader..." << std::endl;
    waterShader->attachShader(GL_VERTEX_SHADER, "shaders/water_vertex.glsl");
    std::cout << "Compiling water fragment shader..." << std::endl;
    waterShader->attachShader(GL_FRAGMENT_SHADER, "shaders/water_fragment.glsl");
    if(!waterShader->compile()) std::cout << "Error compiling water shader!" << std::endl;
    
    widthLocation = waterShader->getUniformLocation("width");
    heightLocation = waterShader->getUniformLocation("height");
    reflectionLocation = waterShader->getUniformLocation("reflectionTexture");
    refractionLocation = waterShader->getUniformLocation("refractionTexture");
    
    // Objects for testing FBO

    secondPass = new Shader();
    secondPass->attachShader(GL_VERTEX_SHADER, "shaders/simple_vertex.glsl");
    secondPass->attachShader(GL_FRAGMENT_SHADER, "shaders/simple_textured_fragment.glsl");
    if(!secondPass->compile()) std::cout << "Error compiling second pass shader!" << std::endl;
    
    skybox = new Object();
    skyboxTexture = new Texture();
    skyboxTexture->loadFromFile("assets/textures/sky.jpg");

    skybox->loadFromObj("assets/sphere.obj");
    skybox->setShader(secondPass);
    skybox->setTexture(skyboxTexture);
    skybox->scale(100);

    quad = new Object();
    quad->loadVertices(vertices, texcoords, normals, indices, 4, 6);

    fbo = new FBO(800, 600);
    reflectFBO = new FBO(800, 600);
    refractFBO = new FBO(800, 600);

    

    glutKeyboardFunc(&keyPressed);
    glutDisplayFunc(testHeightfield);
    glutTimerFunc(1000 / 60, timer, 0);

    //Start GLUT main loop
	glutMainLoop();

    return 0;
}