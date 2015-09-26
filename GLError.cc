#include "GLError.h"

#include <GL/glew.h>

void PrintGLError(const std::string& id) {
    std::cout << id << ": ";
  switch(glGetError()) {
    case GL_NO_ERROR:
        std::cout << "GL_NO_ERROR";
      break;
    case GL_INVALID_ENUM:
      std::cout << "GL_INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      std::cout << "GL_INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      std::cout << "GL_INVALID_OPERATION";
      break;
    case GL_STACK_OVERFLOW:
      std::cout << "GL_STACK_OVERFLOW";
      break;
    case GL_STACK_UNDERFLOW:
      std::cout << "GL_STACK_UNDERFLOW";
      break;
    case GL_OUT_OF_MEMORY:
      std::cout << "GL_OUT_OF_MEMORY";
      break;
    case GL_TABLE_TOO_LARGE:
      std::cout << "GL_TABLE_TOO_LARGE";
      break;
   }
  std::cout << std::endl;
}
