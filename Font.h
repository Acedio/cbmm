#ifndef FONT_H
#define FONT_H

#include <memory>
#include <string>
#include <vector>

#include "TextureManager.h"

struct Character {
  // These are in texture coords
  vec2f lower_left = {0,0}, upper_right = {1,1};
  // This is in character coords, with height = 1 character coord
  float width = 0;
};

// Okay, direction we want to take this... I'm thinking creat a Text object that
// will handle all the character positioning. It'll have an AddCharacter fn
// that'll throw the character's info onto the end(s) of the Attrib arrays.
// Seems like all the attribute arrays need to be consecutive, so maybe we have
// a fixed maximum number of characters (24*3 + room?). Actually, that's
// incorrect. You can have multiple array buffers according to
// http://stackoverflow.com/questions/14249634/opengl-vaos-and-multiple-buffers
// So maybe just keep a vector for position and a vector for texture position.
// Should be able to just change the start index when text scrolls and stuff.
// Maybe look into circular buffers? That way we should only ever have to do
// two draw calls (one from [head, n] and one from [0, tail]).
// Maybe glMultiDrawArrays?

class Font {
 public:
  static std::unique_ptr<Font> MakeFont(const PixelData& pixel_data);
  Character GetCharacter(char c) const;
 private:
  Font() : characters_(256) {}
  std::vector<Character> characters_;
};

#endif  // FONT_H
