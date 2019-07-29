#include <SDL.h>
#include <SDL_image.h>

#include <cassert>
#include <iostream>

#include "TextureManager.h"

TextureManager::TextureManager() {
  next_unused_ref = 0;
  TextureRef ref = getUnusedRef();

  // make the default magenta checkerboard texture

  GLfloat color[12] = {1, 0, 1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 1, 0, 1};
  GLuint texture;

  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, color);

  glBindTexture(GL_TEXTURE_2D, 0);

  textures[ref] = texture;
}

TextureManager::~TextureManager() {
  for (std::pair<TextureRef, GLuint> ref_texture : textures) {
    glDeleteTextures(1, &ref_texture.second);
  }
}

TextureRef TextureManager::getUnusedRef() {
  TextureRef ref = next_unused_ref;
  next_unused_ref += 1;
  return ref;
}

std::unique_ptr<PixelData> LoadToPixelData(std::string filename) {
  std::unique_ptr<PixelData> pd = std::unique_ptr<PixelData>(new PixelData());

  SDL_Surface *surface;

  surface = IMG_Load(filename.c_str());
  if (!surface) {
    std::cout << "Unable to load texture \"" + filename + "\"." << std::endl;
    return nullptr;
  }
  pd->bpp = surface->format->BytesPerPixel;
  if (pd->bpp == 4) {
    if (surface->format->Rmask == 0x000000ff) {
      pd->format = GL_RGBA;
    } else {
      pd->format = GL_BGRA;
    }
  } else if (pd->bpp == 3) {
    if (surface->format->Rmask == 0x000000ff) {
      pd->format = GL_RGB;
    } else {
      pd->format = GL_BGR;
    }
  } else {
    std::cout << "Texture \"" << filename
              << "\" does not have enough channels. It has " << pd->bpp
              << " channels." << std::endl;
    return nullptr;
  }
  pd->w = surface->w;
  pd->h = surface->h;
  pd->data.reserve(pd->w * pd->h * pd->bpp);
  // Flip the image vertically because glTexImage2D expects the first pixel to
  // be the lower left.
  for (int y = pd->h - 1; y >= 0; --y) {
    for (int x = 0; x < pd->w * pd->bpp; ++x) {
      pd->data.push_back(((Uint8 *)surface->pixels)[y * pd->w * pd->bpp + x]);
    }
  }
  SDL_FreeSurface(surface);
  return pd;
}

// level = -1 for auto-mipmapping
TextureRef TextureManager::LoadTexture(std::string filename, int level) {
  TextureRef ref;
  std::map<std::string, TextureRef>::iterator fileref =
      filenames.find(filename);
  if (fileref != filenames.end()) {  // if we already have a reference to this
                                     // texture, return the ref
    ref = fileref->second;
    refcounts[ref] += 1;
  } else {  // otherwise we should load it
    std::unique_ptr<PixelData> pd = LoadToPixelData(filename);
    assert(pd.get());
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    if (level == -1) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_NEAREST);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (level == -1) {
      gluBuild2DMipmaps(GL_TEXTURE_2D, pd->bpp, pd->w, pd->h, pd->format,
                        GL_UNSIGNED_BYTE, pd->data.data());
    } else {
      // TODO: Look into using GL_RGB if we have no alpha channel?
      glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, pd->w, pd->h, 0,
                   pd->format, GL_UNSIGNED_BYTE, pd->data.data());
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    ref = getUnusedRef();

    textures[ref] = texture;  // store the stats about our new texture
    filenames[filename] = ref;
    refcounts[ref] = 1;
  }
  return ref;
}

TextureRef TextureManager::LoadTilemapTexture(const TileMap& tilemap) {
  GLuint texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  int w = tilemap.GetWidth();
  int h = tilemap.GetHeight();
  std::vector<GLubyte> map_data;
  // Textures are positive-up
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      map_data.push_back((GLubyte)tilemap.At(x, y));
    }
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, w, h, 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, map_data.data());

  if (glGetError() == GL_NO_ERROR) {
    std::cout << "Loaded tilemap successfully" << std::endl;
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  TextureRef ref = getUnusedRef();

  textures[ref] = texture;  // store the stats about our new texture
  // TODO: Find out if filenames needs to be updated here, and if so, how.
  refcounts[ref] = 1;

  return ref;
}

void TextureManager::UnloadTexture(TextureRef ref) {
  std::map<TextureRef, int>::iterator refcount;
  if (ref != 0 &&
      (refcount = refcounts.find(ref)) !=
          refcounts.end()) {  // if it's not the default texture and the
                              // reference exists in our store
    refcount->second -= 1;
    if (refcount->second <= 0) {  // if the texture is no longer being used by
                                  // any objects, so we can really remove it
      refcounts.erase(refcount);
      textures.erase(ref);
      for (std::map<std::string, TextureRef>::iterator i = filenames.begin();
           i != filenames.end(); ++i) {
        if (i->second == ref) {
          filenames.erase(i);
          break;
        }
      }
    }
  }
}

void TextureManager::BindTexture(TextureRef ref, int unit) {
  if (unit == 0) {
    glActiveTexture(GL_TEXTURE0);
  } else {
    glActiveTexture(GL_TEXTURE1);
  }

  std::map<TextureRef, GLuint>::iterator tex = textures.find(ref);
  if (tex != textures.end()) {
    glBindTexture(GL_TEXTURE_2D, tex->second);
  } else {
    glBindTexture(GL_TEXTURE_2D, textures[0]);
  }
}
