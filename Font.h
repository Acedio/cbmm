#ifndef FONT_H
#define FONT_H

#include <memory>
#include <string>
#include <vector>

#include "TextureManager.h"

struct Character {
  int x = 0, y = 0;
  int width = 0;
};

class Font {
 public:
  static std::unique_ptr<Font> MakeFont(const PixelData& pixel_data);
  Character GetCharacter(char c);
 private:
  Font() : characters_(256) {}
  std::vector<Character> characters_;
};

#endif  // FONT_H
