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
    ivec2 tilePos = ivec2(TexCoord);
    vec2 coordFloor = floor(TexCoord);
    vec2 tileTexCoord = vec2(TexCoord - coordFloor);

    uint tile_index = texelFetch(tilemap, tilePos, 0).r;
    // Invert y because textures have normal y direction.
    vec2 tile = vec2(mod(tile_index, 16), 15-(tile_index/16));

    vec2 tilesetTexCoord = (tile.xy + tileTexCoord)/float(tilesetSize);

    outputColor = texture(tileset, tilesetTexCoord);
    //outputColor = vec4(float(tilePos.x)/32, float(tilePos.y)/24, 0, 1);
}
