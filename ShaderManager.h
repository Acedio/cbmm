#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <vector>
#include <GL/glew.h>

using namespace std;

GLuint CreateShader(GLenum shaderType, const char *shaderSource);

GLuint OpenShader(GLenum shaderType, const char *shaderFilename);

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

class Program {
public:
    virtual void Setup() = 0;
    bool Load(char const *vertexShaderFile, char const *fragmentShaderFile);
    void Use();
protected:
    GLuint id, vertexShader, fragmentShader;
};

class MapProgram : public Program {
public:
    MapProgram();
    void Setup();
private:
    float mapOffsetX, mapOffsetY;
};

class ShaderManager {
private:
    vector<Program*> programs;
public:
    ShaderManager();
    ~ShaderManager();
    int AddProgram(char const *vertexShaderFile, char const *fragmentShaderFile);
    void UseProgram(int pid);
    void ClearProgram();
};

#endif
