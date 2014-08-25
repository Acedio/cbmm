#include <cmath>

#include "Physics.h"

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

bool Physics::RectMapCollision(const Rect& rect, vec2f* fix) {
    vec2f contact_pt{rect.upperLeft.x + (rect.w/2.0), rect.upperLeft.y - rect.h};

    float map_x = floor(contact_pt.x);
    float map_y = floor(contact_pt.y);

    int tile_type = tile_map.At(contact_pt.x, contact_pt.y);
    if (tile_type == TILE_BLOCK) {
        fix->x = 0;
        fix->y = (map_y + 1) - contact_pt.y;

        // Get the TileType of the above tile to check if we've now entered a slope.
        tile_type = tile_map.At(contact_pt.x, contact_pt.y + 1);
        if (tile_type == TILE_SLOPE_01) {
            fix->y += contact_pt.x - map_x;
        } else if (tile_type == TILE_SLOPE_10) {
            fix->y += 1 - (contact_pt.x - map_x);
        }

        return true;
    } else if (tile_type == TILE_SLOPE_01) {
        float dist_from_slope = (contact_pt.y - map_y) - (contact_pt.x - map_x);
        if (dist_from_slope < 0) {
            fix->x = 0;
            fix->y = -dist_from_slope;
            return true;
        }
        return false;
    } else if (tile_type == TILE_SLOPE_10) {
        float dist_from_slope = (contact_pt.y - map_y) - (1 - (contact_pt.x - map_x));
        if (dist_from_slope < 0) {
            fix->x = 0;
            fix->y = -dist_from_slope;
            return true;
        }
        return false;
    }

    return false;
}

const Rect* Physics::GetBodyRect(size_t i) {
    if (i < bodies.size()) {
        return &bodies.at(i).bbox;
    } else {
        return nullptr;
    }
}
