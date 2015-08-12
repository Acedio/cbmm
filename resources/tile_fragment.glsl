#version 410

in vec2 TexCoord;

out vec4 outputColor;

uniform sampler2D tileset;
uniform sampler2D tilemap;
uniform vec2 offset;
//uniform int tilesetSize;

void main()
{
    int tilesetSize = 4;
    ivec2 tilePos = ivec2(TexCoord + 0);
	vec2 coordFloor = floor(TexCoord + 0);
    vec2 tileTexCoord = vec2(TexCoord - coordFloor);

    vec4 tile = texelFetch(tilemap, tilePos, 0);

    vec2 tilesetTexCoord = (floor(tile.xy*(tilesetSize-1)) + tileTexCoord)/float(tilesetSize);

    outputColor = texture(tileset, tilesetTexCoord);
	
	//outputColor = vec4(1,0,0,1);
}
