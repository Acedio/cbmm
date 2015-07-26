#ifndef GRAPHICS_H
#define GRAPHICS_H

class Display {
 public:
  Display(unsigned int width, unsigned int height, unsigned int bpp);
  ~Display();
  void Clear();
  void Swap();

 private:
  SDL_Window *window;
  unsigned int screen_width;
  unsigned int screen_height;
  unsigned int screen_bpp;
};

#endif
