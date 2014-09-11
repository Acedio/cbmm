#include <cmath>

#include "Physics.h"

namespace {
bool PointMapBelow(const TileMap& tile_map, const vec2f& contact_pt, vec2f* fix){
    float map_y = floor(contact_pt.y);

    int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
    if (tile_type == TILE_BLOCK) {
        fix->y = (map_y + 1) - contact_pt.y;

        return true;
    }

    return false;
}

bool PointMapSlope(const TileMap& tile_map, const vec2f& contact_pt, vec2f* fix){
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
        float dist_from_slope = (contact_pt.y - map_y) - (1 - (contact_pt.x - map_x));
        if (dist_from_slope < 0) {
            fix->y = -dist_from_slope;
            return true;
        }
        return false;
    }

    return false;
}

bool PointMapSide(const TileMap& tile_map, const vec2f& contact_pt, vec2f* fix) {
    int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
    if (tile_type == TILE_BLOCK) {
        // Might want to split this up into two functions...
        fix->x = round(contact_pt.x) - contact_pt.x;
        return true;
    }

    return false;
}
}

bool Physics::RectMapCollision(const Rect& rect, vec2f* fix) {
    int tile_type = tile_map.At(rect.upperLeft.x + (rect.w/2.0), rect.upperLeft.y - rect.h);
    if (tile_type == TILE_SLOPE_01 || tile_type == TILE_SLOPE_10) {
        return PointMapSlope(tile_map, {rect.upperLeft.x + (rect.w/2.0), rect.upperLeft.y - rect.h + fix->y}, fix);
    }

    bool collided_below = PointMapBelow(tile_map, {rect.upperLeft.x, rect.upperLeft.y - rect.h}, fix) || 
        PointMapBelow(tile_map, {rect.upperLeft.x + rect.w, rect.upperLeft.y - rect.h}, fix);

    bool collided_side = PointMapSide(tile_map, {rect.upperLeft.x, rect.upperLeft.y - rect.h + fix->y}, fix) ||
        PointMapSide(tile_map, {rect.upperLeft.x + rect.w, rect.upperLeft.y - rect.h + fix->y}, fix) ||
        PointMapSide(tile_map, {rect.upperLeft.x, rect.upperLeft.y + fix->y}, fix) ||
        PointMapSide(tile_map, {rect.upperLeft.x + rect.w, rect.upperLeft.y + fix->y}, fix);
    
    return collided_below || collided_side;
}

void Physics::Update(double dt) {
    for (auto& body : bodies) {
        if (body.enabled) {
            Rect new_rect = body.bbox;
            new_rect.upperLeft = body.bbox.upperLeft + body.vel*dt + vec2f{dt/2.0, 0};

            vec2f fix;
            if (RectMapCollision(new_rect, &fix)) {
                new_rect.upperLeft += fix;
                body.vel = vec2f{0,0};
            } else {
                body.vel += vec2f{0,-9*dt};
            }
            body.bbox = new_rect;
        }
    }
}

const Rect* Physics::GetBodyRect(size_t i) {
    if (i < bodies.size()) {
        return &bodies.at(i).bbox;
    } else {
        return nullptr;
    }
}
