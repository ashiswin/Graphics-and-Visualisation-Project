#include <iostream>
#include <fstream>

#include <shader.h>
#include <light.h>

Shader::Shader() {
    program = glCreateProgram();
}

void Shader::attachShader(GLenum shaderType, const char* filename) {
    std::ifstream shaderSource(filename);
    std::string str((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
    const char *c_str = str.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    GLint shaderCompiled;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
    if(shaderCompiled == GL_FALSE)
    {
        std::cout << "Failed to compile shader!" << std::endl;
        GLint blen = 0;	
        GLsizei slen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &blen);       
        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);
            glGetShaderInfoLog(shader, blen, &slen, compiler_log);
            std::cout << "compiler_log: " << compiler_log << std::endl;
            free (compiler_log);
        }
    }

    glAttachShader(program, shader);
}

bool Shader::compile() {
    bindAttribute(0, "position");
    bindAttribute(1, "texCoords");
    bindAttribute(2, "normal");

    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) 
    {
        char logBuf[1024];
        int len;

        glGetProgramInfoLog(program, 1024, &len, logBuf);
        std::cout << "Could not link shaders due to error: " << logBuf << std::endl;
        return false;
    }
    modelMatrixLocation = getUniformLocation("modelMatrix");
    projectionMatrixLocation = getUniformLocation("projectionMatrix");
    viewMatrixLocation = getUniformLocation("viewMatrix");

    lightDirectionLocation = getUniformLocation("lightDirection");
    lightColorLocation = getUniformLocation("lightColor");
    shineDamperLocation = getUniformLocation("shineDamper");
    reflectivityLocation = getUniformLocation("reflectivity");
    hasNormalMapLocation = getUniformLocation("hasNormalMap");

    normalMapLocation = getUniformLocation("normalSampler");
    textureLocation = getUniformLocation("textureSampler");

    return success == GL_TRUE;
}

void Shader::bindAttribute(int attrib, const char* variableName) {
    glBindAttribLocation(program, attrib, variableName);
}

void Shader::loadModelMatrix(glm::mat4 matrix) {
    glUniformMatrix4fv(modelMatrixLocation, 1, false, &matrix[0][0]);
}

void Shader::loadProjectionMatrix(glm::mat4 matrix) {
    glUniformMatrix4fv(projectionMatrixLocation, 1, false, &matrix[0][0]);
}

void Shader::loadViewMatrix(glm::mat4 matrix) {
    glUniformMatrix4fv(viewMatrixLocation, 1, false, &matrix[0][0]);
}

void Shader::loadSpecularComponents(float shineDamper, float reflectivity) {
    glUniform1f(shineDamperLocation, shineDamper);
    glUniform1f(reflectivityLocation, reflectivity);
}

void Shader::loadLight(DirectionalLight *light) {
    glUniform3fv(lightDirectionLocation, 1, &light->getDirection()[0]);
    glUniform3fv(lightColorLocation, 1, &light->getColor()[0]);
}

void Shader::enableTexture() {
    glUniform1i(textureLocation, 0);
}

void Shader::enableNormalMap() {
    glUniform1f(hasNormalMapLocation, 1);
    glUniform1i(normalMapLocation, 1);
}

void Shader::attach() {
    glUseProgram(program);
}

void Shader::detach() {
    glUseProgram(0);
}

int Shader::getUniformLocation(const char* uniformName) {
    return glGetUniformLocation(program, uniformName);
}