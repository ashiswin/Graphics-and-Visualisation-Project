#include <GL/glew.h>
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <object.h>
#include <texture.h>
#include <shader.h>
#include <camera.h>
#include <terrain.h>
#include <light.h>
#include <fbo.h>
#include <heightfield.h>
#include <lightmesh.h>
#include <skybox.h>

#define WIDTH 150
#define HEIGHT 150
#define DETAIL 400
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

Shader *firstPass;
Shader *secondPass;
Shader *waterShader;
Shader *terrainShader;

GLuint widthLocation, heightLocation;
GLuint reflectionLocation, refractionLocation, planeLocation;
GLuint causticsTextureLocation;
Object *quad;
// Object *skybox;
Object *plane;
Terrain *terrain;
Object *terrainObj;
Object *submarine;

Camera *camera, *reflectCamera, *refractCamera;
Camera *underwaterCamera;
DirectionalLight *light;
Texture *terrainTexture, *submarineTexture;

FBO *fbo, *reflectFBO, *refractFBO;
Heightfield *water;
LightMesh *lightMesh;
Skybox *skybox;

glm::mat4 projectionMatrix;

float vertices[] = {
    -1, 1, 0,
    1, 1, 0,
    1, -1, 0,
    -1, -1, 0
};

float pvertices[] = {
    -10, -2, 0,
    10, -2, 0,
    10, -2, -20,
    -10, -2, -20
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

float pnormals[] = {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
};

int indices[] = {
    0, 1, 3,
    1, 2, 3
};

bool mouseHeld;
int prevY = -1;
int prevX = -1;

void mousePressed(int button, int state, int x, int y) {
    if(button == 0) {
        mouseHeld = state == 0;
        prevY = y;
        prevX = x;
    }
}

void mouseMoved(int x, int y) {
    if(mouseHeld) {
        if(y - prevY > 0) {
            camera->rotate(1, 0, 0);
        }
        else if(y - prevY < 0){
            camera->rotate(-1, 0, 0);
        }
        prevY = y;

        if(x - prevX > 0) {
            camera->rotate(0, 1, 0);
        }
        else if(x - prevX < 0){
            camera->rotate(0, -1, 0);
        }
        prevX = x;
    }
}

void keyPressed(unsigned char c, int x, int y) {
    switch(c) {
        case 'w':
            camera->move(0, .1, 0);
            break;
        case 'd':
            camera->move(.1, 0, 0);
            break;
        case 'a':
            camera->move(-.1, 0, 0);
            break;
        case 's':
            camera->move(0, -.1, 0);
            break;
        case 'v':
            water->addHeight(0.1 + rand()%1, glm::vec2(rand() % DETAIL, rand() % DETAIL));
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
    

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::thf 0: " << err << std::endl;
    }
    
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::thf 1: " << err << std::endl;
    }

    water->bindNormalMap();
    FBO *caustics = lightMesh->draw(projectionMatrix, camera, light);
    water->unbindNormalMap();

    // std::cout << camera->getPosition().y << std::endl;

    // Render reflection
    reflectFBO->bind();
    #ifdef __APPLE__
        glViewport(0, 0, 800, 600);
    #else
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    #endif

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // if (camera->getPosition().y >= 0) {
        skybox->setClippingPlane(glm::vec4(0, 1, 0, 1));
        skybox->draw(projectionMatrix, reflectCamera->getViewMatrix());
        skybox->disableClippingPlane();

    reflectFBO->unbind();

    // Render refraction
    refractFBO->bind();
    #ifdef __APPLE__
        glViewport(0, 0, 800, 600);
    #else
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    #endif

    glClear(GL_COLOR_BUFFER_BIT);

    if (camera->getPosition().y < 0) {
        skybox->setClippingPlane(glm::vec4(0, 1, 0, 1));
        skybox->draw(projectionMatrix, reflectCamera->getViewMatrix());
        skybox->disableClippingPlane();
    } else {
        skybox->setClippingPlane(glm::vec4(0, -1, 0, 1));
        skybox->draw(projectionMatrix, camera->getViewMatrix());
        skybox->disableClippingPlane();

        terrainTexture->bind(GL_TEXTURE0);

        terrainShader->attach();
        terrainShader->loadProjectionMatrix(projectionMatrix);
        terrainShader->loadViewMatrix(camera->getViewMatrix());
        terrainShader->enableTexture();
        glUniform1i(causticsTextureLocation, 2);
        caustics->bindColorTexture(GL_TEXTURE2);

        terrainObj->setShader(terrainShader);
        terrainObj->draw();
        terrainShader->detach();    
        terrainTexture->unbind();

        caustics->unbindColorTexture();
    }

    refractFBO->unbind();

    // Begin final pass
    #ifdef __APPLE__
        glViewport(0, 0, 800, 600);
    #else
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    #endif

    skybox->draw(projectionMatrix, camera->getViewMatrix());

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

    terrainTexture->bind(GL_TEXTURE0);

    terrainShader->attach();
    terrainShader->loadProjectionMatrix(projectionMatrix);
    terrainShader->loadViewMatrix(camera->getViewMatrix());
    terrainShader->enableTexture();
    glUniform1i(causticsTextureLocation, 2);
    caustics->bindColorTexture(GL_TEXTURE2);

    terrainObj->setShader(terrainShader);
    terrainObj->draw();
    terrainShader->detach();    
    terrainTexture->unbind();

    caustics->unbindColorTexture();

    // secondPass->attach();
    // secondPass->loadProjectionMatrix(projectionMatrix);
    // secondPass->loadViewMatrix(camera->getViewMatrix());
    // secondPass->enableTexture();
    // submarineTexture->bind(GL_TEXTURE0);
    // submarine->draw();
    // submarineTexture->unbind();
    // secondPass->detach();

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

    #ifdef __APPLE__
        glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
        glutInitWindowSize(800, 600);
        glutCreateWindow( "OpenGL" );
    #else
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);	
        glutInitContextVersion (4, 1);
        glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
        glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        glutCreateWindow("OpenGL 4.1");
        glutFullScreen();
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    #endif
    
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
    camera->move(0.5, 1.5, 1.5);
    camera->rotate(20, 0, 0);

    reflectCamera = new Camera();
    reflectCamera->rotate(-30, 180, 0.0f);
    reflectCamera->position = glm::vec3(0, -10, 5);

    light = new DirectionalLight(glm::vec3(0, -1, 0), glm::vec3(1, 1, 1));
    
    projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);

    water = new Heightfield(DETAIL, 2);
    lightMesh = new LightMesh(1000);
    skybox = new Skybox();
    
    plane = new Object();
    plane->loadVertices(pvertices, texcoords, pnormals, indices, 4, 6);

    firstPass = new Shader();
    firstPass->attachShader(GL_VERTEX_SHADER, "shaders/simple_vertex.glsl");
    firstPass->attachShader(GL_FRAGMENT_SHADER, "shaders/simple_fragment.glsl");
    if(!firstPass->compile()) std::cout << "Error compiling first pass shader!" << std::endl;
    
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
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Error in Object::main: " << err << std::endl;
    }
    // Objects for testing FBO

    secondPass = new Shader();
    secondPass->attachShader(GL_VERTEX_SHADER, "shaders/simple_vertex.glsl");
    secondPass->attachShader(GL_FRAGMENT_SHADER, "shaders/simple_textured_fragment.glsl");
    if(!secondPass->compile()) std::cout << "Error compiling second pass shader!" << std::endl;

    // shader for terrain
    terrainShader = new Shader();
    terrainShader->attachShader(GL_VERTEX_SHADER, "shaders/simple_vertex.glsl");
    terrainShader->attachShader(GL_FRAGMENT_SHADER, "shaders/terrain_textured_fragment.glsl");
    if(!terrainShader->compile()) std::cout << "Error compiling terrain shader!" << std::endl;

    causticsTextureLocation = terrainShader->getUniformLocation("causticsTexture");

    planeLocation = secondPass->getUniformLocation("plane");

    quad = new Object();
    quad->loadVertices(vertices, texcoords, normals, indices, 4, 6);

    #ifdef __APPLE__
        fbo = new FBO(800, 600);
        reflectFBO = new FBO(800, 600);
        refractFBO = new FBO(800, 600);
    #else
        fbo = new FBO(WINDOW_WIDTH, WINDOW_HEIGHT);
        reflectFBO = new FBO(WINDOW_WIDTH, WINDOW_HEIGHT);
        refractFBO = new FBO(WINDOW_WIDTH, WINDOW_HEIGHT);
    #endif

    terrain = new Terrain(1000);
    terrainObj = terrain->generateGeometry();
    // terrainObj->scale(20);
    // terrainObj->move(-0.5, -0.5, -0.5);
    glm::vec3 position = glm::vec3(120, -3, -100);
    firstPass->loadCenterBowl(position);
    terrainObj->setShader(terrainShader);

    terrainTexture = new Texture();
    terrainTexture->loadFromFile("assets/textures/tile2.jpg");

    glutKeyboardFunc(&keyPressed);
    glutMouseFunc(&mousePressed);
    glutMotionFunc(&mouseMoved);
    glutDisplayFunc(testHeightfield);
    glutTimerFunc(1000 / 60, timer, 0);

    //Start GLUT main loop
	glutMainLoop();

    return 0;
}