#include <GL/glew.h>
#include <fstream>
#include <iostream>

#include "ShaderManager.h"

using namespace std;

GLuint CreateShader(GLenum shaderType, const char *shaderSource) {
    GLuint shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, &shaderSource, NULL);

    glCompileShader(shader);

    GLint compiled = 0;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    
    if (compiled == GL_FALSE) {
        cout << "Error encountered in ";

        switch (shaderType) {
            case GL_VERTEX_SHADER: cout << "vertex"; break;
            case GL_FRAGMENT_SHADER: cout << "fragment"; break;
            default: cout << "unknown"; break;
        }

        cout << " shader:" << endl;

        int log_length = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        char *log = new char[log_length];

        glGetShaderInfoLog(shader, log_length, &log_length, log);

        cout << log << endl;

        delete[] log;
    }

    return shader;
}

GLuint OpenShader(GLenum shaderType, const char *shaderFilename) {
    ifstream shaderFile(shaderFilename);

    if (!shaderFile.good()) {
        return 0;
    }

    shaderFile.seekg(0, ios::end);
    int length = shaderFile.tellg();
    shaderFile.seekg(0, ios::beg);

    // + 1 for \0
    char *shaderSource = new char[length + 1];

    shaderFile.read(shaderSource, length);
    shaderSource[length] = 0;

    shaderFile.close();

    GLuint shaderRef = CreateShader(shaderType, shaderSource);

    delete[] shaderSource;

    return shaderRef;
}

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint linked = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    
    if (linked == GL_FALSE) {
        cout << "Error encountered while linking the shader program." << endl;

        int log_length = 0;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

        char *log = new char[log_length];

        glGetProgramInfoLog(program, log_length, &log_length, log);

        cout << log << endl;

        delete[] log;
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

ShaderManager::ShaderManager() {
    program = SetupProgram("vertex.glsl", "tile_fragment.glsl");
}

ShaderManager::~ShaderManager() {
}

GLuint ShaderManager::SetupProgram(char const *vertexShaderFile, char const *fragmentShaderFile) {
    vertexShader = OpenShader(GL_VERTEX_SHADER, vertexShaderFile);
    fragmentShader = OpenShader(GL_FRAGMENT_SHADER, fragmentShaderFile);

	if (vertexShader == 0 || fragmentShader == 0) {
		return 0;
	}
	
    program = CreateProgram(vertexShader, fragmentShader);

    return program;
}

void ShaderManager::UseProgram(float x, float y) {
    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "tileset"), 0);
    glUniform1i(glGetUniformLocation(program, "tilemap"), 1);
    glUniform2f(glGetUniformLocation(program, "offset"), x, y);
}

void ShaderManager::ClearProgram() {
    glUseProgram(0);
}
