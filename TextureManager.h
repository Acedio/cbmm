#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <glew.h>

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <map>
using namespace std;

typedef unsigned int TextureRef;

class TextureManager{
public:
	TextureManager();
	~TextureManager();

	TextureRef LoadTexture(string filename, int level);
	void UnloadTexture(TextureRef ref);

	void BindTexture(TextureRef ref, int unit);
private:
	map<string, TextureRef> filenames;
	map<TextureRef, GLuint> textures;
	map<TextureRef, int> refcounts;
	TextureRef next_unused_ref;
};

#endif
