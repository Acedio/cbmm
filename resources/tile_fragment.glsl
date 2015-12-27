#version 410

in vec2 TexCoord;

out vec4 outputColor;

uniform sampler2D tileset;
uniform usampler2D tilemap;
uniform vec2 offset;
//uniform int tilesetSize;

void main()
{
    int tilesetSize = 16;
    vec2 offsetCoord = TexCoord + offset;
    // floor() here since it seems like casting to ivec2 rounds towards zero.
    ivec2 tilePos = ivec2(floor(offsetCoord));
    vec2 coordFrac = mod(offsetCoord, 1);

    uint tile_index = texelFetch(tilemap, tilePos, 0).r;
    // Invert y because textures have normal y direction.
    ivec2 tile = ivec2(mod(tile_index, 16), 15-(tile_index/16));

    vec2 tilesetTexCoord = tile.xy + coordFrac;
    // Clamp away from the (1,1) border so we don't accidentally draw bits of
    // the neighboring tiles.
    vec2 clampedCoord = clamp(tilesetTexCoord, tile, tile + vec2(0.98,0.98));
    vec2 normalizedCoord = clampedCoord/float(tilesetSize);

    outputColor = texture(tileset, normalizedCoord);
    //outputColor = vec4(float(tilePos.x)/32, float(tilePos.y)/24, 0, 1);
}
