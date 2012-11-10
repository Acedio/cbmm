#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>

#include "Display.h"

using namespace std;

Display::Display(unsigned int width, unsigned int height, unsigned int bpp) {
    screen_width = width;
    screen_height = height;
    screen_bpp = bpp;

    SDL_Init(SDL_INIT_EVERYTHING);

    if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)) {
        cout << "Error setting up GL." << endl;
    }

    SDL_Surface *surface = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, SDL_OPENGL);

    if(!surface) {
        cout << "Error opening surface: " << SDL_GetError() << endl;
    }

    GLenum glewErr = glewInit();
    
    if(glewErr != GLEW_OK) {
        cout << "Error: " << glewGetErrorString(glewErr) << endl;
        return;
    }

    glClearColor(0,0,0,1);

	glViewport(0, 0, screen_width, screen_height);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Display::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::Swap() {
    SDL_GL_SwapBuffers();
}

Display::~Display() {
    SDL_Quit();
}
