#include <cassert>
#include <cmath>
#include <iostream>

#include "Physics.h"

namespace {
bool PointMapBelow(const TileMap& tile_map, const vec2f& contact_pt,
                   double* y_fix) {
  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_BLOCK) {
    *y_fix = floor(contact_pt.y + 1) - contact_pt.y;

    // If we're still in a tile, return the larger fix so we choose the correct
    // (small) fix on the other axis.
    if (tile_map.At(contact_pt.x, contact_pt.y + 1) == TILE_BLOCK) {
      *y_fix += 1;
    }

    return true;
  }

  *y_fix = 0;
  return false;
}

bool PointMapAbove(const TileMap& tile_map, const vec2f& contact_pt,
                   double* y_fix) {
  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_BLOCK) {
    *y_fix = floor(contact_pt.y) - contact_pt.y - 0.01;

    if (tile_map.At(contact_pt.x, contact_pt.y - 1) == TILE_BLOCK) {
      *y_fix -= 1;
    }

    return true;
  }

  *y_fix = 0;
  return false;
}

bool PointMapLeft(const TileMap& tile_map, const vec2f& contact_pt,
                  double* x_fix) {
  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_BLOCK) {
    *x_fix = floor(contact_pt.x + 1) - contact_pt.x;
    if (tile_map.At(contact_pt.x + 1, contact_pt.y) == TILE_BLOCK) {
      *x_fix += 1;
    }
    return true;
  }

  *x_fix = 0;
  return false;
}

bool PointMapRight(const TileMap& tile_map, const vec2f& contact_pt,
                   double* x_fix) {
  int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (tile_type == TILE_BLOCK) {
    *x_fix = floor(contact_pt.x) - contact_pt.x - 0.01;
    if (tile_map.At(contact_pt.x - 1, contact_pt.y) == TILE_BLOCK) {
      *x_fix -= 1;
    }
    return true;
  }

  *x_fix = 0;
  return false;
}

// TODO: These might belong in TileMap.cc
double heightAtX(double tilespace_x, int tile_type) {
  switch (tile_type) {
    case TILE_SLOPE_01:
      return tilespace_x;
    case TILE_SLOPE_10:
      return 1 - tilespace_x;
    case TILE_SLOPE_05:
      return 0.5 * tilespace_x;
    case TILE_SLOPE_51:
      return 0.5 + 0.5 * tilespace_x;
    case TILE_SLOPE_15:
      return 1 - 0.5 * tilespace_x;
    case TILE_SLOPE_50:
      return 0.5 - 0.5 * tilespace_x;
    default:
      assert(false);
      return -10;
  }
}

bool is_slope(TileType tile_type) {
  return tile_type == TILE_SLOPE_01 || tile_type == TILE_SLOPE_10 ||
         tile_type == TILE_SLOPE_05 || tile_type == TILE_SLOPE_51 ||
         tile_type == TILE_SLOPE_15 || tile_type == TILE_SLOPE_50;
};

// TODO: Velocity after @fix should be parallel to the slope so jittering
// doesn't occur.
bool PointMapSlope(const TileMap& tile_map, const vec2f& contact_pt,
                   double* y_fix) {
  double map_x = floor(contact_pt.x);
  double map_y = floor(contact_pt.y);

  TileType tile_type = tile_map.At(contact_pt.x, contact_pt.y);
  if (is_slope(tile_type)) {
    double dist_from_slope =
        (contact_pt.y - map_y) - heightAtX(contact_pt.x - map_x, tile_type);
    if (dist_from_slope < 0) {
      *y_fix = -dist_from_slope;
      return true;
    }
    *y_fix = 0;
    return false;
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

bool Physics::XCollision(const Rect& rect, double* x_fix) {
  double tmp;
  PointMapRight(tile_map, {rect.lowerLeft.x + rect.w, rect.lowerLeft.y}, x_fix);
  PointMapRight(tile_map,
                {rect.lowerLeft.x + rect.w, rect.lowerLeft.y + rect.h}, &tmp);
  *x_fix = min(*x_fix, tmp);

  if (*x_fix == 0) {
    PointMapLeft(tile_map, {rect.lowerLeft.x, rect.lowerLeft.y}, x_fix);
    PointMapLeft(tile_map, {rect.lowerLeft.x, rect.lowerLeft.y + rect.h}, &tmp);
    *x_fix = max(*x_fix, tmp);
  }

  return *x_fix != 0;
}

bool Physics::YCollision(const Rect& rect, double* y_fix) {
  double tmp;
  PointMapBelow(tile_map, {rect.lowerLeft.x, rect.lowerLeft.y}, y_fix);
  PointMapBelow(tile_map, {rect.lowerLeft.x + rect.w, rect.lowerLeft.y}, &tmp);
  *y_fix = max(*y_fix, tmp);

  if (*y_fix == 0) {
    PointMapAbove(tile_map, {rect.lowerLeft.x, rect.lowerLeft.y + rect.h},
                  y_fix);
    PointMapAbove(tile_map,
                  {rect.lowerLeft.x + rect.w, rect.lowerLeft.y + rect.h},
                  &tmp);
    *y_fix = min(*y_fix, tmp);
  }

  return *y_fix != 0;
}

bool Physics::RectMapCollision(const Rect& rect, const vec2f& last_pos,
                               vec2f* fix) {
  // TODO: Remove this from outside this function (only set to zero once).
  *fix = {0,0};

  bool was_on_slope =
      is_slope(tile_map.At(last_pos.x + (rect.w / 2.0), last_pos.y));

  if (!was_on_slope) {
    // We weren't on a slope, check X
    Rect x_only = rect;
    x_only.lowerLeft.y = last_pos.y;
    XCollision(x_only, &fix->x);
  }

  if (PointMapSlope(tile_map,
                    {rect.lowerLeft.x + (rect.w / 2.0), last_pos.y},
                    &fix->y)) {
    // fix should be based on rect.lowerLeft, so we need to correct from
    // last_pos.
    fix->y =  (last_pos.y + fix->y) - rect.lowerLeft.y;
    return true;
  }

  if (!is_slope(tile_map.At(rect.lowerLeft.x + (rect.w / 2.0),
                            rect.lowerLeft.y))) {
    // Not on a slope, check Y w.r.t. block map.
    Rect x_fixed = rect;
    x_fixed.lowerLeft.x += fix->x;
    YCollision(x_fixed, &fix->y);
  } else {
    PointMapSlope(tile_map,
                  {rect.lowerLeft.x + (rect.w / 2.0), rect.lowerLeft.y},
                  &fix->y);
  }

  return fix->x != 0 || fix->y != 0;
}

vector<std::unique_ptr<Event>> Physics::Update(Seconds dt,
                                               const vector<Entity>& entities) {
  vector<std::unique_ptr<Event>> collisions;

  for (size_t i = 0; i < entities.size(); ++i) {
    auto* body = entities[i].GetComponent<Body>();
    assert(body);
    if (body->enabled) {
      body->last_pos = body->bbox.lowerLeft;
      std::cout << body->last_pos.x << " " << body->last_pos.y << std::endl;
      body->bbox.lowerLeft += body->vel * dt;
      // tilemap collision
      vec2f fix{0, 0};
      if (RectMapCollision(body->bbox, body->last_pos, &fix)) {
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
