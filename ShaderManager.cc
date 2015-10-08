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
      case GL_VERTEX_SHADER:
        cout << "vertex";
        break;
      case GL_FRAGMENT_SHADER:
        cout << "fragment";
        break;
      default:
        cout << "unknown";
        break;
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

  // I think these may need to stick around for the above logging, but not
  // sure...
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);

  if (linked == GL_FALSE) {
    glDeleteProgram(program);
    program = 0;
  }

  return program;
}

GLuint LoadProgram(char const *vertexShaderFile,
                   char const *fragmentShaderFile) {
  GLuint vertexShader = OpenShader(GL_VERTEX_SHADER, vertexShaderFile);
  GLuint fragmentShader = OpenShader(GL_FRAGMENT_SHADER, fragmentShaderFile);

  if (vertexShader == 0 || fragmentShader == 0) {
    return false;
  }

  GLuint id = CreateProgram(vertexShader, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return id;
}

void Program::Use() { glUseProgram(id); }

void TextureProgram::Setup() {
  glUniform1i(texture_uniform_, 0);
}

void MapProgram::Setup() {
  // TODO: This seems strange... Are these texture references? Maybe texture
  // units?
  glUniform1i(tileset_uniform_, 0);
  glUniform1i(tilemap_uniform_, 1);
  glUniform2f(offset_uniform_, map_offset_.x, map_offset_.y);
}

std::unique_ptr<MapProgram> MapProgram::Make() {
  std::unique_ptr<MapProgram> program(new MapProgram());
  program->id = LoadProgram("resources/texture_vertex.glsl",
                            "resources/tile_fragment.glsl");
  if (!program->id) {
    return nullptr;
  }

  program->tileset_uniform_ = glGetUniformLocation(program->id, "tileset");
  program->tilemap_uniform_ = glGetUniformLocation(program->id, "tilemap");
  program->offset_uniform_ = glGetUniformLocation(program->id, "offset");

  return program;
}

std::unique_ptr<TextureProgram> TextureProgram::Make() {
  std::unique_ptr<TextureProgram> program(new TextureProgram());
  program->id = LoadProgram("resources/texture_vertex.glsl",
                            "resources/texture_fragment.glsl");
  if (!program->id) {
    return nullptr;
  }

  program->texture_uniform_ = glGetUniformLocation(program->id, "texture");

  return program;
}

std::unique_ptr<ColorProgram> ColorProgram::Make() {
  std::unique_ptr<ColorProgram> program(new ColorProgram());
  program->id = LoadProgram("resources/color_vertex.glsl",
                            "resources/color_fragment.glsl");
  if (!program->id) {
    return nullptr;
  }

  return program;
}
