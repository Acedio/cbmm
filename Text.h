#ifndef TEXT_H
#define TEXT_H

#include <vector>

#include <GL/glew.h>

#include "Font.h"
#include "Geometry.h"

class Text {
 public:
  static std::unique_ptr<Text> MakeText(const Font* font);
  void AddCharacter(char c);
  void Draw() const;
 private:
  Text(const Font* font) : font_(font) {}

  // In character coords, where character height = 1 character coord
  vec2f last_pos_;

  GLuint vertex_array_object_;

  // Can probably combine these.
  GLuint position_buffer_;
  GLuint texture_coord_buffer_;

  std::vector<GLfloat> positions_;
  std::vector<GLfloat> texture_coords_;

  // Does not own. Must outlive this object.
  const Font* font_;
  std::vector<char> characters_;
};

#endif  // TEXT_H
