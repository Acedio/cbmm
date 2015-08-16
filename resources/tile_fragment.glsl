#version 410

in vec2 TexCoord;

out vec4 outputColor;

uniform sampler2D tileset;
uniform sampler2D tilemap;
uniform vec2 offset;
//uniform int tilesetSize;

void main()
{
    int tilesetSize = 16;
    ivec2 tilePos = ivec2(TexCoord + 0);
	  vec2 coordFloor = floor(TexCoord + 0);
    vec2 tileTexCoord = vec2(TexCoord - coordFloor);

    vec4 float_tile = texelFetch(tilemap, tilePos, 0);
    int tile_index = int(256.0 * float_tile.r);
    vec2 tile = vec2(mod(tile_index, 16), 15-tile_index/16);

    vec2 tilesetTexCoord = (tile.xy + tileTexCoord)/float(tilesetSize);

    outputColor = texture(tileset, tilesetTexCoord);
}
