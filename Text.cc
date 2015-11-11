#include "Text.h"

std::unique_ptr<Text> Text::MakeText(const Font* font) {
  std::unique_ptr<Text> text = std::unique_ptr<Text>(new Text(font));
  glGenBuffers(1, &text->position_buffer_);
  glGenBuffers(1, &text->texture_coord_buffer_);
  return text;
}

void Text::AddCharacter(char c) {
  characters_.push_back(c);

  Character character = font_->GetCharacter(c);

  // Top left triangle postions
  positions_.push_back(last_pos_.x);
  positions_.push_back(last_pos_.y);
  positions_.push_back(last_pos_.x);
  positions_.push_back(last_pos_.y - 1);
  positions_.push_back(last_pos_.x + character.width);
  positions_.push_back(last_pos_.y);

  // Bottom right triangle positions
  positions_.push_back(last_pos_.x + character.width);
  positions_.push_back(last_pos_.y);
  positions_.push_back(last_pos_.x);
  positions_.push_back(last_pos_.y - 1);
  positions_.push_back(last_pos_.x + character.width);
  positions_.push_back(last_pos_.y - 1);

  last_pos_.x += character.width;

  // Top left triangle tex coords
  texture_coords_.push_back_(character.lower_left.x);
  texture_coords_.push_back_(character.upper_right.y);
  texture_coords_.push_back_(character.lower_left.x);
  texture_coords_.push_back_(character.lower_left.y);
  texture_coords_.push_back_(character.upper_right.x);
  texture_coords_.push_back_(character.upper_right.y);

  // Bottom right triangle tex coords
  texture_coords_.push_back_(character.upper_right.x);
  texture_coords_.push_back_(character.upper_right.y);
  texture_coords_.push_back_(character.lower_left.x);
  texture_coords_.push_back_(character.lower_left.y);
  texture_coords_.push_back_(character.upper_right.x);
  texture_coords_.push_back_(character.lower_left.y);
}

void Text::Draw(const vec2f&, int) const {
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
  // Given GL_STATIC_DRAW and GL_DYNAMIC_DRAW, it seems like this is probably
  // shouldn't be reuploaded every time :P
  glBufferData(GL_ARRAY_BUFFER, positions_.size(), positions_.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buffer_);
  glBufferData(GL_ARRAY_BUFFER, texture_coords_.size(), positions_.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLES, 0, characters_.size());
  glDisable(GL_BLEND);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
