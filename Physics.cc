#include <cassert>
#include <cmath>

#include "Physics.h"

namespace {
bool PointMapBelow(const TileMap& tile_map, const vec2f& contact_pt,
                   float* y_fix) {
  float map_y = floor(contact_pt.y);

  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_BLOCK) {
    *y_fix = (map_y + 1) - contact_pt.y;

    // If we're still in a tile, return the larger fix so we choose the correct
    // (small) fix on the other axis.
    if (tile_map.At(contact_pt.x, contact_pt.y + 1) == TILE_BLOCK) {
      *y_fix += 1;
    }

    return true;
  }

  return false;
}

bool PointMapAbove(const TileMap& tile_map, const vec2f& contact_pt,
                   float* y_fix) {
  float map_y = floor(contact_pt.y);

  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_BLOCK) {
    *y_fix = map_y - contact_pt.y;

    if (tile_map.At(contact_pt.x, contact_pt.y - 1) == TILE_BLOCK) {
      *y_fix -= 1;
    }

    return true;
  }

  return false;
}

// TODO: Velocity after @fix should be parallel to the slope so jittering
// doesn't occur.
bool PointMapSlope(const TileMap& tile_map, const vec2f& contact_pt,
                   vec2f* fix) {
  float map_x = floor(contact_pt.x);
  float map_y = floor(contact_pt.y);

  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_SLOPE_01) {
    float dist_from_slope = (contact_pt.y - map_y) - (contact_pt.x - map_x);
    if (dist_from_slope < 0) {
      fix->y = -dist_from_slope;
      return true;
    }
    return false;
  } else if (tile_type == TILE_SLOPE_10) {
    float dist_from_slope =
        (contact_pt.y - map_y) - (1 - (contact_pt.x - map_x));
    if (dist_from_slope < 0) {
      fix->y = -dist_from_slope;
      return true;
    }
    return false;
  }

  return false;
}

bool PointMapLeft(const TileMap& tile_map, const vec2f& contact_pt,
                  float* x_fix) {
  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_BLOCK) {
    *x_fix = floor(contact_pt.x + 1) - contact_pt.x;
    if (tile_map.At(contact_pt.x + 1, contact_pt.y) == TILE_BLOCK) {
      *x_fix += 1;
    }
    return true;
  }

  return false;
}

bool PointMapRight(const TileMap& tile_map, const vec2f& contact_pt,
                   float* x_fix) {
  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_BLOCK) {
    *x_fix = floor(contact_pt.x) - contact_pt.x;
    if (tile_map.At(contact_pt.x - 1, contact_pt.y) == TILE_BLOCK) {
      *x_fix -= 1;
    }
    return true;
  }

  return false;
}

// Returns true if a and b overlap: A [  { ]  } B
// @fix is set to the distance A must move to not overlap B.
bool AxisCheck(double a1, double a2, double b1, double b2, double* fix) {
  if (b1 < a1) {
    // Flip so a1 is always left of b1
    if (AxisCheck(b1, b2, a1, a2, fix)) {
      *fix = -*fix;
      return true;
    }
  } else {
    *fix = b1 - a2;
    if (*fix < 0) {
      return true;
    }
  }
  return false;
}
}  // namespace

// Returns true if @first and @second collide. @fix is set to the correction
// that @first must make to no longer collide with @second.
bool Physics::RectRectCollision(const Rect& first, const Rect& second,
                                vec2f* fix) {
  double x_fix, y_fix;
  if (AxisCheck(first.lowerLeft.x, first.lowerLeft.x + first.w,
                second.lowerLeft.x, second.lowerLeft.x + second.w, &x_fix) &&
      AxisCheck(first.lowerLeft.y, first.lowerLeft.y + first.h,
                second.lowerLeft.y, second.lowerLeft.y + second.h, &y_fix)) {
    if (abs(x_fix) < abs(y_fix)) {
      fix->x = x_fix;
      fix->y = 0;
    } else {
      fix->x = 0;
      fix->y = y_fix;
    }
    return true;
  }
  return false;
}

bool Physics::RectMapCollision(const Rect& rect, vec2f* fix) {
  int tile_type = tile_map.At(rect.lowerLeft.x + (rect.w / 2.0),
                                    rect.lowerLeft.y);
  if (tile_type == TILE_SLOPE_01 || tile_type == TILE_SLOPE_10) {
    return PointMapSlope(tile_map, {rect.lowerLeft.x + (rect.w / 2.0),
                                    rect.lowerLeft.y + fix->y},
                         fix);
  }

  float x_fix = 0, y_fix = 0;

  bool collided_side =
      PointMapLeft(tile_map, {rect.lowerLeft.x, rect.lowerLeft.y}, &x_fix) ||
      PointMapLeft(tile_map, {rect.lowerLeft.x, rect.lowerLeft.y + rect.h},
                   &x_fix) ||
      PointMapRight(tile_map, {rect.lowerLeft.x + rect.w, rect.lowerLeft.y},
                    &x_fix) ||
      PointMapRight(tile_map,
                    {rect.lowerLeft.x + rect.w, rect.lowerLeft.y + rect.h},
                    &x_fix);

  bool collided_above =
      PointMapAbove(tile_map, {rect.lowerLeft.x, rect.lowerLeft.y + rect.h},
                    &y_fix) ||
      PointMapAbove(tile_map,
                    {rect.lowerLeft.x + rect.w, rect.lowerLeft.y + rect.h},
                    &y_fix);

  bool collided_below =
      PointMapBelow(tile_map, {rect.lowerLeft.x, rect.lowerLeft.y},
                    &y_fix) ||
      PointMapBelow(tile_map,
                    {rect.lowerLeft.x + rect.w, rect.lowerLeft.y},
                    &y_fix);

  if (abs(x_fix) > abs(y_fix)) {
    *fix = {0, y_fix};
  } else {
    *fix = {x_fix, 0};
  }

  return collided_below || collided_above || collided_side;
}

vector<std::unique_ptr<Event>> Physics::Update(Seconds,
                                               const vector<Entity>& entities) {
  vector<std::unique_ptr<Event>> collisions;

  for (size_t i = 0; i < entities.size(); ++i) {
    auto* body = entities[i].GetComponent<Body>();
    assert(body);
    if (body->enabled) {
      // tilemap collision
      vec2f fix{0, 0};
      if (RectMapCollision(body->bbox, &fix)) {
        std::unique_ptr<CollisionEvent> collision(new CollisionEvent());
        collision->first = i;
        collision->second = MAP_BODY_ID;
        collision->fix = fix;
        collisions.push_back(std::move(collision));
      }

      // rect rect collisions
      vec2f rect_fix{0, 0};
      auto collide_to = body;
      collide_to++;
      for (size_t target_i = i + 1; target_i < entities.size(); ++target_i) {
        auto* target_body = entities[target_i].GetComponent<Body>();
        assert(target_body);
        if (target_body->enabled &&
            RectRectCollision(body->bbox, target_body->bbox, &rect_fix)) {
          std::unique_ptr<CollisionEvent> collision(new CollisionEvent());
          collision->first = i;
          collision->second = target_i;
          collision->fix = rect_fix;
          collisions.push_back(std::move(collision));
        }
      }
    }
  }

  return collisions;
}
