#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <GL/glew.h>

GLuint CreateShader(GLenum shaderType, const char *shaderSource);

GLuint OpenShader(GLenum shaderType, const char *shaderFilename);

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

class ShaderManager {
private:
    GLuint program, vertexShader, fragmentShader;
    GLuint textureUniform;
public:
    ShaderManager();
    ~ShaderManager();
    GLuint SetupProgram(char const *vertexShaderFile, char const *fragmentShaderFile);
    void UseProgram();
    void ClearProgram();
};

#endif
